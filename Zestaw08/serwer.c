/* ==============================================================
 * plik serwer.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 11.05.2021
 * ostatnia modyfikacja: 17.05.2021
 * ==============================================================
 */

#define _XOPEN_SOURCE 500
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#include "msg.h"
#include "srv.h"

//funkcje

void terminuj(int sig_no);
void usun_kolejke(void);

//operacje

#define suma(a,b) a+b
#define roznica(a,b) suma(a,-b)
#define iloczyn(a,b) a*b
double iloraz(double a,double b);

//makro do wyjścia z procesu
#define leave(message)         \
    do{                        \
        sig;                   \
        perror(message);       \
        exit(EXIT_FAILURE);    \
    }while(0)

//makro wypisujące rodzaj procesu
#define sig printf("[SERVER] ")

/* makro do wylosowania liczby mikrosekund do przespania
 *
 * przedział od 1 us do 1s
 */
#define random (rand()%1000000)+1

int deskryptor;

extern int errno;

int main (int argc, const char *argv[]) {

    srand(time(NULL)); //ustaw ziarno

    sig;
    printf("Rozpoczynam!\n");

    deskryptor = msgCreate(SRV_NAME); //utwórz kolejke

    printf("Kolejka %s o deskryptorze %d\n",SRV_NAME,deskryptor);

    if (atexit(usun_kolejke) == -1) { //atexit kolejki
        sig;
        perror("atexit error");
        _exit(EXIT_FAILURE);
    }
    
    if(signal(SIGINT,terminuj) == SIG_ERR) //ustaw obsługe sigint
        leave("signal error");

    msgInfo(deskryptor);

    while (1) {
        
        komunikat msg;
        msg.priority = 1;

        usleep(random);

        msgRecv(deskryptor,&msg); //otrzymaj komunikat

        sig;
        printf("Komunikat otrzymany: \n%s\n",msg.at);

        //odczytaj PID klienta
        int PID;
        char operacja[MAX_SIZE];

        if (sscanf(msg.at,"%d %s",&PID,operacja) < 2) { //podziel operacje
            errno = EINVAL;
            leave("incorrect message");
        }

        char klient[16];
        int deskryptor_klient;

        //otwórz jego kolejke
        sprintf(klient,"/%d",PID);

        deskryptor_klient = msgOpen(klient);

        //wykonaj działanie
        double x, y, wynik;
        char o;

        if (sscanf(operacja,"%lf %c %lf",&x,&o,&y) < 3) {
            errno = EINVAL;
            leave("incorrect operation");
        }

        switch (o)
        {
        case '+':
            wynik = suma(x,y);
            break;

        case '-':
            wynik = roznica(x,y);
            break;

        case '*':
            wynik = iloczyn(x,y);
            break;

        case '/':
            wynik = iloraz(x,y);
            break;
        
        default:
            errno = EINVAL;
            leave("invalid operator");
        }

        printf("Wynik: %lf\n",wynik);

        //prześlij je na jego kolejke
        sprintf(msg.at,"%lf",wynik);

        msgSend(deskryptor_klient,&msg);

        //zamknij kolejke
        msgClose(deskryptor_klient);
        

    }

    exit(EXIT_FAILURE);
}

void terminuj(int sig_no) {
    printf("\n");
    sig;
    printf("Wychodzenie za pomocą SIGINT.\n");
    exit(EXIT_SUCCESS);
}

void usun_kolejke(void) {
    msgClose(deskryptor);
    msgRm(SRV_NAME);
}

double iloraz(double a,double b) {
    if (b == 0) {              
        errno = EINVAL;        
        leave("division by 0");
    }                          
    return a/b;
}