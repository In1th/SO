/* ==============================================================
 * plik powielacz.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 24.04.2021
 * ostatnia modyfikacja: 27.04.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

#include "sem.h"

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//flagi funkcji open
#define RESET_TAGS 01101
#define PERMISSIONS 0664
#define WRTIE_ONLY 01
#define READ_ONLY 00

void sprzatanie(void);
void ucieczka(int sig);

const char* name = "/sem";

const char* file = "numer.txt";

extern int errno;

//makro do wyjścia z procesu
#define leave(message)       \
    {                        \
        perror(message);     \
        exit(EXIT_FAILURE); \
    }

int main(int argc, char *argv[]) {

    //sprawdzanie czy przekazano wszystkie argumenty
    if (argc != 4) {
        errno = INVALID_ARGUMENTS;
        leave("argument number error")
    }  

    //ustawianie funkcji przy wywołaniu exit
    if (atexit(sprzatanie) == -1)
        leave("atexit error")

    //tworzenie semaforu
    semid semafor = semCreate(name,1);
    printf("Adres semafora: %p ",&semafor);
    semInfo(semafor);
    semClose(semafor);

    //ustawienie przechwytywania sygnału SIGINT
    if (signal(SIGINT,ucieczka) == SIG_ERR)
        leave("signal error")

    int process_number = atoi(argv[2]),
        process_count,
        dsc;

    //otwórz plik "numer.txt"
    if ((dsc = open(file,RESET_TAGS,PERMISSIONS)) == -1)
        leave("reset/create file error")

    //wpisz do tego pliku 0
    if (write(dsc,"0",1) == -1)
        leave("write error")

    close(dsc);

    //otwórz argv[2] procesów potomnych fork + exec
    for (process_count = 0; process_count < process_number; process_count++) {
        switch (fork()) {
        case -1:
            leave("fork error")
            break;
        
        case 0:
            execl(argv[1],argv[1],argv[3],NULL);
            leave("exec error")
        }
    }

    //czekanie na procesy potomne
    while (process_count > 0){
        printf("Czekam\n");
        if (wait(NULL) == -1)
            leave("wait error")
        process_count--;
    }

    //sprawdź czy plik "numer.txt" sie zgadza
    if ((dsc = open(file,READ_ONLY)) == -1)
        leave("open error")

    char *bufor = (char *)calloc(8,sizeof(char));

    if(read(dsc,bufor,8) == -1)
        leave("read error")

    printf("Liczba koncowa: %s\n",bufor);

    if (atoi(bufor) == atoi(argv[2]) * atoi(argv[3])) {
        printf("Operacja zakonczona powodzeniem!\n");
        free(bufor);
        exit(EXIT_SUCCESS);
    }
    else {
        printf("Operacja zakonczona porazka!\n");
        free(bufor);
        exit(EXIT_FAILURE);
    }

}

void sprzatanie(void) {
    semRemove(name);
}

void ucieczka(int sig) {
    exit(EXIT_SUCCESS);
}