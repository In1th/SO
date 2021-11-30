/* ==============================================================
 * plik wykluczanie.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 18.05.2021
 * ostatnia modyfikacja: 18.05.2021
 * ==============================================================
 */

#define _XOPEN_SOURCE 500
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define LICZBA_WATKOW 5

#define UTWORZONO "\033[34mUtworzono\033[37m"
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

    char nr_iteracji;
    volatile int licznik_prywatny;
    char sekcja;

} atrubuty;

void *watek(void *ptr);
void wypisz(void);
void koniec(void);

pthread_t watki[LICZBA_WATKOW];
atrubuty w_atr[LICZBA_WATKOW];

pthread_mutex_t mutex;

volatile int licznik = 0;

const int iterations = 6;

int main(int argc, const char *argv[]) {

    srand(time(NULL));

    if (pthread_mutex_init(&mutex, NULL) == -1)
        leave("create mutex");

    if (atexit(koniec) == -1) 
        leave("atexit error");


    int i;

    for (i = 0; i < LICZBA_WATKOW; i++) {

        w_atr[i].nr_iteracji = 0;
        w_atr[i].sekcja = 0;


        if (pthread_create(&watki[i],NULL,watek,&w_atr[i]) == -1)
            leave("thread creation");

    }


    for (i = 0; i < LICZBA_WATKOW; i++) {
        if (pthread_join(watki[i],NULL) == -1)
            leave("thread join");
    }    

    wypisz();

    exit(EXIT_SUCCESS);
}

void *watek(void *ptr) {

    usleep(RANDOM);

    atrubuty *atrs = ptr;
    atrs -> sekcja = 1;
    wypisz();  
    usleep(RANDOM);

    int i;

    for (i = 0; i < iterations; i++) {

        atrs -> nr_iteracji = i;
        atrs -> sekcja = 2;
        wypisz();
        if (pthread_mutex_lock(&mutex) == -1)
            leave("mutex lock error");

        atrs -> sekcja = 5;

        atrs -> licznik_prywatny = licznik;

        wypisz();

        usleep(RANDOM);

        atrs -> licznik_prywatny++;

        licznik = atrs -> licznik_prywatny;

        if (pthread_mutex_unlock(&mutex) == -1)
            leave("mutex unlock error");

        atrs -> sekcja = 3;
        usleep(RANDOM);

    }

    atrs -> sekcja = 4;
    wypisz();  
    usleep(RANDOM);

    pthread_exit(NULL);

}

void wypisz(void) {
    printf("\033c");
    printf("\033[1:1H");
    printf("\033[1mWątek Nr Sekcja prywatna \033[32GSekcja krytyczna\033[0m\n");

    int i;

    for(i = 0; i < LICZBA_WATKOW; i++) {
        printf("\033[%d:1H",i+3);
        printf("Wątek %d",i+1);
        printf("\033[10G");
        switch (w_atr[i].sekcja)
        {
        case 1:
            printf("%s",UTWORZONO);
            break;

        case 2:
            printf("%s",CZEKAM);
            break;

        case 3:
            printf("%s",SPIE);
            break;

        case 4:
            printf("%s",KONIEC);
            break;

        case 5:
            printf("\033[32G");
            printf("%s(%d/%d)",SEKCJA_KRYTYCZNA,w_atr[i].nr_iteracji,licznik);
            break;
        
        default:
            break;
        }

        printf("\033[%d:1H",7+LICZBA_WATKOW+i);        
        printf("-> Utworzono wątek nr %d pod ID %ld\n",i+1,watki[i]);

        printf("\n");
    }

    printf("\033[%d:1H",6+LICZBA_WATKOW);
    printf("---> Utworzono mutex pod adresem %p\n",(void *)&mutex);
    printf("\033[%d:1H",7+LICZBA_WATKOW+LICZBA_WATKOW);
}

void koniec(void) {
    pthread_mutex_destroy(&mutex);
}