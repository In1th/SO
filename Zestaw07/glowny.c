/* ==============================================================
 * plik glowny.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 3.05.2021
 * ostatnia modyfikacja: 10.05.2021
 * ==============================================================
 */

#include <wait.h>
#include <signal.h>

#include "bufor.h"
#include "shmem.h"
#include "sem.h"

//funkcje atexit
void zamknij_obiekt(void);
void zamknij_producenta(void);
void zamknij_konsumenta(void);

//funkcja do obsługi sygnału
void ucieczka(int sig);

//makro do wyjścia z procesu
#define leave(message)         \
    do{                        \
        perror(message);       \
        exit(EXIT_FAILURE);    \
    } while(0)

//zmienne globalne

const char *kons = "/kons";
const char *prod = "/prod";
const char *obiekt = "/obiekt";

const int buf_size = 4;

semid semafory[] = {NULL,NULL};
int deskryptor = 0;

int main(int argc, char *argv[]) {

    //utwórz obiekt pamięci dzielonej
    if ((deskryptor = shmCreate(obiekt,sizeof(cykliczny))) == -1) 
        leave("shm open error");

    cykliczny *bufor;

    //zmapuj obiekt pamięci dzielonej
    if ((bufor = shmMap(deskryptor,sizeof(cykliczny))) == MAP_FAILED) {
        close(deskryptor);
        leave("shm map error");
    }

    //ustaw atexit dla obiektu pamięci dzielonej
    if (atexit(zamknij_obiekt) == -1) {
        shmClose(bufor,deskryptor,sizeof(cykliczny));
        leave("atexit shm error");
    }

    bufInit(bufor,buf_size); //inicjalizuj bufor

    shmClose(bufor,deskryptor,sizeof(cykliczny)); //zamknij obiekt

    semafory[0] = semCreate(prod,buf_size); //utwórz semafor producenta

    //ustaw atexit semafora producenta
    if (atexit(zamknij_producenta) == -1) {
        semClose(semafory[0]);
        leave("atexit prod error");
    }

    semClose(semafory[0]); //zamknijj semafor producenta

    semafory[1] = semCreate(kons,0); //utwórz semafor konsumenta

    //ustaw atexit semafora konsumenta
    if (atexit(zamknij_konsumenta) == -1) {
        semClose(semafory[1]);
        leave("atexit kons error");
    }

    semClose(semafory[1]); //zamknij semafor producenta

    //ustawienie przechwytywania sygnału SIGINT
    if (signal(SIGINT,ucieczka) == SIG_ERR)
        leave("signal error");

    printf("Konfiguracja zakończona!\n");
    
    int i;

    for (i = 0; i < 2; i++){ //twórz procesy potomne

        switch (fork())
        {
        case -1:
            leave("fork error");

        case 0:
            if (i == 0) execl("./producent.x","producent.x",NULL);
            if (i == 1) execl("./konsument.x","konsument.x",NULL);
            _exit(EXIT_FAILURE);

    }
    }

    while (i > 0) //czekaj na potomków
    {
        printf("czekam\n");
        if (wait(NULL) == -1)
            leave("wait error");
        i--;
    }

    exit(EXIT_SUCCESS);

}

void zamknij_obiekt(void) {
    shmRm(obiekt);
}
void zamknij_producenta(void) {
    semRemove(prod);
}
void zamknij_konsumenta(void) {
    semRemove(kons);
}

void ucieczka(int sig) {
    exit(EXIT_FAILURE);
}