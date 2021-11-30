/* ==============================================================
 * plik piekarnia.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 26.05.2021
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
#define RANDOM (rand() % 1000000) + 10000

typedef struct {

    char nr_watku;
    char sekcja;

} atrubuty;

void *watek(void *ptr);
void wypisz(atrubuty *atrs);

void zakoncz(int sig);

int max();
int porownaj(int i, int j);

pthread_t watki[LICZBA_WATKOW];
atrubuty w_atr[LICZBA_WATKOW];

static volatile char wybieranie[LICZBA_WATKOW];
static volatile int numer[LICZBA_WATKOW];

int main(int argc, const char *argv[]) {

    srand(time(NULL));

    if (signal(SIGINT,zakoncz) == SIG_ERR)
        leave("signal error");

    printf("\033c");
    printf("\033[1:1H");
    printf("\033[1mWątek Nr Sekcja prywatna \033[32GSekcja krytyczna\033[0m\n");

    int i;

    for (i = 0; i < LICZBA_WATKOW; i++) {

        wybieranie[i] = 0;
        numer[i] = 0;

        w_atr[i].nr_watku = i;
        w_atr[i].sekcja = 0;

        if (pthread_create(&watki[i],NULL,watek,&w_atr[i]) == -1)
            leave("thread creation");

        printf("\033[%d:1H",7+LICZBA_WATKOW+i);        
        printf("-> Utworzono wątek nr %d pod ID %ld\n",i+1,watki[i]);

    }


    for (i = 0; i < LICZBA_WATKOW; i++) {
        if (pthread_join(watki[i],NULL) == -1)
            leave("thread join");
    }    

    printf("\033[%d:1H",7+2*LICZBA_WATKOW);

    exit(EXIT_SUCCESS);


}

void *watek(void *ptr) {

    usleep(RANDOM);

    atrubuty *atrs = ptr;
    int i = atrs -> nr_watku, j;
    atrs -> sekcja = 1;
    wypisz(atrs);  
    usleep(RANDOM);

    while(1) {

        //wybieranie numeru
        atrs -> sekcja = 3;
        wypisz(atrs);
        usleep(RANDOM);
        wybieranie[i] = 1; 
        numer[i] = max() + 1;
        wybieranie[i] = 0;

        //czekanie
        atrs -> sekcja = 2;
        wypisz(atrs);
        for (j = 0; j < LICZBA_WATKOW; j++) {
            while (wybieranie[j]);
            while (numer[j] != 0 && porownaj(j,i));
        }

        //sekcja krytycnza
        atrs -> sekcja = 6;
        wypisz(atrs);
        usleep(RANDOM);

        numer[i] = 0;

        //śpij
        atrs -> sekcja = 4;
        wypisz(atrs);
        usleep(2*RANDOM);

    }

    pthread_exit(NULL);

}

void wypisz(atrubuty *atrs) {

    int i = atrs -> nr_watku;

    printf("\033[%d:1H",i+3);
    printf("\033[2K\r");
    printf("Wątek %d",i+1);
    printf("\033[10G");
    switch (atrs -> sekcja)
        {
        case 1:
            printf("%s",UTWORZONO);
            break;

        case 2:
            printf("%s",CZEKAM);
            break;

        case 3:
            printf("%s",WYBIERAM);
            break;

        case 4:
            printf("%s",SPIE);
            break;

        case 5:
            printf("%s",KONIEC);
            break;

        case 6:
            printf("\033[32G");
            printf("%s(%d)",SEKCJA_KRYTYCZNA,numer[i]);
            break;
        
        default:
            break;
        }

        printf("\n");
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
    int i;
    for (i = 0; i < LICZBA_WATKOW; i++) {
        w_atr[i].sekcja = 5;
        wypisz(&w_atr[i]);
    }
    printf("\033[%d:1H",7+2*LICZBA_WATKOW);
    exit(EXIT_SUCCESS);
}