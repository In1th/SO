/* ==============================================================
 * plik piekarnia_licznik.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 27.05.2021
 * ostatnia modyfikacja: 26.05.2021
 * ==============================================================
 */

#define _XOPEN_SOURCE 500
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define LICZBA_WATKOW 5
#define ITERACJE 10000000

#define UTWORZONO "\033[34mUtworzono\033[37m"
#define WYBIERAM "\033[33mWybieram\033[37m"
#define CZEKAM "\033[32mCzekam\033[37m"
#define SPIE "\033[35mŚpię\033[37m"
#define KONIEC "\033[91mZakończono\033[37m"
#define SEKCJA_KRYTYCZNA "\033[31mSekcja krytyczna\033[37m"

//makro do wyjścia z procesu
#define leave(message)         \
    do{                        \
        perror(message);       \
        exit(EXIT_FAILURE);    \
    } while(0)

/* makro do wylosowania liczby mikrosekund do przespania
 *
 * przedział od 1 us do 1s
 */
#define RANDOM (rand() % 10) + 1

typedef struct {

    char nr_watku;
    int nr_iteracji;

} atrubuty;

void *watek(void *ptr);

void lock(atrubuty *atrs);
void unlock(atrubuty *atrs);

void zakoncz(int sig);

int max();
int porownaj(int i, int j);

pthread_t watki[LICZBA_WATKOW];
atrubuty w_atr[LICZBA_WATKOW];

static volatile char wybieranie[LICZBA_WATKOW];
static volatile int numer[LICZBA_WATKOW];

volatile int licznik = 0;

int main(int argc, const char *argv[]) {

    srand(time(NULL));

    if (signal(SIGINT,zakoncz) == SIG_ERR)
        leave("signal error");

    int i;

    for (i = 0; i < LICZBA_WATKOW; i++) {

        wybieranie[i] = 0;
        numer[i] = 0;

        w_atr[i].nr_watku = i;
        w_atr[i].nr_iteracji = 0;

        if (pthread_create(&watki[i],NULL,watek,&w_atr[i]) == -1)
            leave("thread creation");

    }


    for (i = 0; i < LICZBA_WATKOW; i++) {
        if (pthread_join(watki[i],NULL) == -1)
            leave("thread join");
    }    


    zakoncz(0);
    exit(EXIT_SUCCESS);


}

void *watek(void *ptr) {

    //usleep(RANDOM);

    atrubuty *atrs = ptr;  
    //usleep(RANDOM);

    for(atrs -> nr_iteracji = 0; atrs -> nr_iteracji < ITERACJE;) {

        atrs -> nr_iteracji++;

        lock(atrs);

        //sekcja krytycnza

        licznik = licznik + 1;

        unlock(atrs);

    }

    pthread_exit(NULL);

}



int max() {

    int output = numer[0], i;

    for (i = 1; i < LICZBA_WATKOW; i++) {
        if (output < numer[i])
            output = numer[i];
    }

    return output;

}

int porownaj(int a, int b) {

    if (numer[a] < numer[b])
        return 1;
    else if (numer[a] == numer[b] && a < b)
        return 1;
    else
        return 0;

}

void zakoncz(int sig) {
    int i, licz = 0;
    for (i = 0; i < LICZBA_WATKOW; i++) {
        licz += w_atr[i].nr_iteracji;
    }

    printf("\nPrzewidywane: %d Zapisane w liczniku: %d\n",licz,licznik);
    exit(EXIT_SUCCESS);
}

void lock(atrubuty *atrs) {

    int i = atrs -> nr_watku, j;

     //wybieranie numeru
    //usleep(RANDOM)
    wybieranie[i] = 1;

    //__sync_synchronize();
    numer[i] = max() + 1;

    //__sync_synchronize();
    wybieranie[i] = 0;

    //czekanie
    for (j = 0; j < LICZBA_WATKOW; j++) {
        while (wybieranie[j]);
        while (numer[j] != 0 && porownaj(j,i));
    }

}

void unlock(atrubuty *atrs) {

    int i = atrs -> nr_watku;

    numer[i] = 0;

    //śpij
    //usleep(2*RANDOM);

}