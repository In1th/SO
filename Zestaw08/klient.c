/* ==============================================================
 * plik klient.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 11.05.2021
 * ostatnia modyfikacja: 17.05.2021
 * ==============================================================
 */

#define _XOPEN_SOURCE 500
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "msg.h"
#include "srv.h"

//funkcje

void terminuj(int sig_no);
void usun_kolejke(void);

char *pobierz(char *buf,int size);

//makro do wyjścia z procesu
#define leave(message)         \
    do{                        \
        sig;                   \
        perror(message);       \
        exit(EXIT_FAILURE);    \
    }while(0)

//makro wypisujące rodzaj procesu
#define sig printf("[KLIENT %d] ",PID)

/* makro do wylosowania liczby mikrosekund do przespania
 *
 * przedział od 1 us do 1s
 */
#define random (rand()%1000000)+1

//zmienne globalne

int PID;
int deskryptor;
char nazwa[16];

extern int errno;

int main (int argc, const char *argv[]) {

    srand(time(NULL)); //ustaw ziarno

    PID = getpid();

    sig;
    printf("Rozpoczynam!\n");

    int wielkosc_nazwy = sprintf(nazwa,"/%d",PID); //stwórz napis /PID

    deskryptor = msgCreate(nazwa); //utwórz kolejke

    printf("Kolejka %s o deskryptorze %d\n",nazwa,deskryptor);

    msgInfo(deskryptor);

    if (atexit(usun_kolejke) == -1) { //atexit kolejki
        errno = EINVAL;
        leave("incorrect message");
    }

    if(signal(SIGINT,terminuj) == SIG_ERR)  //ustaw obsługe sigint
        leave("signal error");

    char operacja[MAX_SIZE - wielkosc_nazwy - 1];

    komunikat msg;

    while(pobierz(operacja,MAX_SIZE) != NULL) { //pobierz z stdin

        msg.priority = 1;
        sprintf(msg.at,"%d %s",PID,operacja); //zapisz komunikat

        int deskryptor_serwer;

        deskryptor_serwer = msgOpen(SRV_NAME); //otwórz kolejke serwera

        msgSend(deskryptor_serwer,&msg); //wyślij komunikat

        usleep(random);

        msgClose(deskryptor_serwer); //zamknij kolejke serwera

        sig;
        printf("Komunikat wysłany: \n%s\n",msg.at);

        msgRecv(deskryptor,&msg); //odczytaj wynik

        sig;
        printf("Komunikat otrzymany: \n%s\n",msg.at);

    }

    printf("\n");
    sig;
    printf("Zakończenie za pomocą Ctrl+D!\n");
    exit(EXIT_SUCCESS);

}

void terminuj(int sig_no) {
    printf("\n");
    sig;
    printf("Wychodzenie za pomocą SIGINT.\n");
    exit(EXIT_SUCCESS);
}

void usun_kolejke(void){
    msgClose(deskryptor);
    msgRm(nazwa);
}

char *pobierz(char *buf,int size) {
    printf("$ ");
    return fgets(buf, size, stdin);
}