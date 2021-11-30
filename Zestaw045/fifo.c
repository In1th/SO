/* ==============================================================
 * plik fifo.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 13.04.2021
 * ostatnia modyfikacja: 16.04.2021
 * ==============================================================
 */

#define _POSIX_SOURCE
#define _XOPEN_SOURCE 500

//biblioteki
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//uprawnienia potoku
#define MODE 0644

/* ------------------------------------------------------------------------------- 
 * Definicje funkcji
 * -------------------------------------------------------------------------------  
 */

/* Funkcja przechwytująca sygnał SIGINT i usuwający poprawnie potok*/
void ucieczka(int sig);

/* Funkcja usuwająca potok przy wyjściu.*/
void sprzatanie(void);

/* ------------------------------------------------------------------------------- 
 * Makrodefinicje
 * -------------------------------------------------------------------------------  
 */

//makro do wyjścia z procesu
#define leave(message)      \
    {                       \
        perror(message);    \
        exit(EXIT_FAILURE); \
    }

/* ------------------------------------------------------------------------------- 
 * Zmienne globalne 
 * -------------------------------------------------------------------------------  
 */

extern int errno; //wykorzystywany do manualnego ustawiania errno

char *pipe_name = NULL; //nazwa potoku nazwanego


int main(int argc, char *argv[])
{

    //ustawianie funkcji przy wywołaniu exit
    if(atexit(sprzatanie) == -1) {
        perror("atexit error");
        _exit(EXIT_FAILURE);
    }

    //sprawdzanie czy przekazano wszystkie argumenty
    if (argc != 4)
    {
        errno = INVALID_ARGUMENTS;
        leave("argument error")
    }

    pipe_name = argv[1]; //ustaw zmienną globalną dla funkcji sprzątającej

    //utworzenie potoku nazwanego
    if (mkfifo(pipe_name, MODE) == -1)
        leave("mkfifo error")

    //ustawienie przechwytywania sygnału SIGINT
    if (signal(SIGINT,ucieczka) == SIG_ERR)
        leave("signal error")

    int i;

    //wywołanie 2 x (fork + exec)
    for (i = 0; i < 2; i++)
    {

        switch (fork())
        {
        case -1: //błąd
            leave("fork error") break;

        case 0: //procesy potomne
            if (i == 0) { //producent
                execl("./producent.x", "./producent.x", argv[1], argv[2], NULL);
                leave("exec error")
            }
            if (i == 1) { //konsument
                execl("./konsument.x", "./konsument.x", argv[1], argv[3], NULL);
                leave("exec error")
            }
        }
    }

    //czekaj na wszystkie procesy potomne
    while (i > 0)
    {
        printf("czekam\n");
        if (wait(NULL) == -1)
            leave("wait error")
        i--;
    }


    exit(EXIT_SUCCESS); //wyjdź
}

void ucieczka(int sig){
    exit(EXIT_FAILURE);
}

void sprzatanie(void)
{
    printf("Usuwam potok!\n");
    
    if (pipe_name != NULL)
        if (unlink(pipe_name) == -1)
            perror("unlink");
}