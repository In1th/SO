/* ==============================================================
 * plik obsluga.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 30.03.2021
 * ostatnia modyfikacja: 30.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "procinfo.h"

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//makro do wyjścia z procesu
#define leave(message) {perror(message); exit(EXIT_FAILURE);}

extern int errno;

void wypisz(int sign){
    printf("Nr sygnalu: %d \n", sign);
}

int main(int argc, const char *argv[]) {

    if (argc != 3) {
        errno = INVALID_ARGUMENTS;
        leave("argument error")
    }

    procinfo(argv[0]);

    char option = argv[1][0];
    int signal_no = atoi(argv[2]);

    switch (option) {
    case 'd':
        if (signal(signal_no,SIG_DFL) == SIG_ERR)
            leave("signal error")
        break;
    
    case 'i':
        if (signal(signal_no,SIG_IGN) == SIG_ERR)
            leave("signal error") 
        break;

    case 'p':
        if (signal(signal_no,wypisz) == SIG_ERR)
            leave("signal error")
        break;
    
    default:
        errno = INVALID_ARGUMENTS;
        leave("flag error")
        break;
    }

    pause();

    return 0;
}