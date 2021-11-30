/* ==============================================================
 * plik producent.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 5.05.2021
 * ostatnia modyfikacja: 10.05.2021
 * ==============================================================
 */

#define _XOPEN_SOURCE 500
#include <time.h>
#include <unistd.h>

#include "bufor.h"
#include "sem.h"
#include "shmem.h"

//funckje atexit
void zamknij_obiekt(void);
void zamknij_producenta(void);
void zamknij_konsumenta(void);
void zamknij_plik(void);

//funkcja wykonująca zadania producenta
void produkuj(void);

//funkcja wypisująca status
void status(towar t);

//makro do wyjścia z procesu
#define leave(message)         \
    do{                        \
        perror(message);       \
        exit(EXIT_FAILURE);    \
    }while(0)

//makro wypisujące rodzaj procesu
#define sig printf("[P] ")

/* makro do wylosowania liczby mikrosekund do przespania
 *
 * przedział od 1 us do 1s
 */
#define random (rand()%1000000)+1

//zmienne globalne

const char *obiekt = "/obiekt";
const char *prod = "/prod";
const char *kons = "/kons";
const char *magazyn = "magazyn.txt";

semid semafory[] = {NULL,NULL};
cykliczny *bufor = NULL;
int deskryptor = 0,
    plik = 0;

int main(int argc, char *argv[]){

    srand(time(NULL)); //ustaw ziarno

    //otwórz obiekt pamięci dzielonej
    sig;
    if ((deskryptor = shmOpen(obiekt)) == -1)
        leave("[p] shm open error");

    //zmapuj obiekt pamięci dzielonej
    sig;
    if ((bufor = shmMap(deskryptor, sizeof(cykliczny))) == MAP_FAILED)
    {
        close(deskryptor);
        leave("[p] shm map error");
    }

    //ustaw atexit dla obiektu pamięci dzielonej
    if (atexit(zamknij_obiekt) == -1)
    {
        shmClose(bufor, deskryptor, sizeof(cykliczny));
        leave("[p] atexit shm error");
    }

    //otwórz semafor producenta
    sig;
    semafory[0] = semOpen(prod);

    //ustaw atexit semafora producenta
    if (atexit(zamknij_producenta) == -1)
    {
        semClose(semafory[0]);
        leave("[p] atexit prod error");
    }

    //otwórz semafor konsumenta
    sig;
    semafory[1] = semOpen(kons);

    //ustaw atexit semafora konsumenta
    if (atexit(zamknij_konsumenta) == -1)
    {
        semClose(semafory[1]);
        leave("[p] atexit kons error");
    }

    //otwórz plik do czytania rzeczy
    if ((plik = open(magazyn,O_RDONLY)) == -1)
        leave("[p] file error");

    //ustaw atexit dla pliku
    if (atexit(zamknij_plik) == -1) {
        close(plik);
        leave("[p] atexit file error");
    }

    printf("[p] Konfiguracja zakończona!\n");

    produkuj();


    exit(EXIT_SUCCESS);

}

void zamknij_obiekt(void) {
    shmClose(bufor,deskryptor,sizeof(cykliczny));
}

void zamknij_producenta(void) {
    semClose(semafory[0]);
}

void zamknij_konsumenta(void) {
    semClose(semafory[1]);
}

void zamknij_plik(void) {
    close(plik);
}

void produkuj(void) {

    int size, bytes, count;

    if ((size = shmSize(plik)) == -1)
        leave("file size error");

    do {

        count = 0;
        towar *t = (towar*)calloc(1,sizeof(towar));

        //opusc swój semafor
        sig;
        semP(semafory[0]);

        usleep(random);

        //sekcja krytyczna
        /* dynamiczne pobieranie pakietu danych
         *
         * pętla pobiera znak po znaku do momentu kiedy:
         *  - napotka znak spacji lub znak nowej linii
         *  - wczyta 8 bajtów (maksymalna pojemność pakietu)
         *  - wczyta 0 bajtów (koniec znaków do wczytywania)
         */
        do
        {
            char *buf = (char*)calloc(2, 1);
            if ((bytes = read(plik, buf, 1)) == -1)
                leave("read file error");

                if (bytes != 0)
                {
                    count++;
                    t -> at[count - 1] = *buf;
                }
            free(buf);
        } while (bytes == 1 && count < 8 && t -> at[count - 1] != ' ' && t -> at[count - 1] != '\n');

        t -> at[9] = '\0';

        size -= count;

        status(*t);

        if (pushBack(bufor,*t) == -1)
            leave("bufor error");

        free(t);

        //podnieś semafor konsumenta
        sig;
        semV(semafory[1]);

    } while (size > 0);

    //podnieś semafor konsumenta
    sig;
    semV(semafory[1]);

}

void status(towar t) {
    printf("[P] towar przesyłany -------\n");
    printf("%s\n",t.at);
    printf("(%ldB)------------------------\n",strlen(t.at));
}
