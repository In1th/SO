/* ==============================================================
 * plik semuse.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 16.04.2021
 * ostatnia modyfikacja: 27.04.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "sem.h"

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//makro do wyjścia z procesu
#define leave(message)       \
    {                        \
        perror(message);     \
        exit(EXIT_FAILURE); \
    }

extern int errno;

int main(int argc, char *argv[]) {

    //sprawdzanie czy przekazano wszystkie argumenty
    if (argc != 2) {
        errno = INVALID_ARGUMENTS;
        leave("main arguments")
    }

    semid semafor;

    switch (argv[1][0])
    {
    case 'i': //utworz semafor
        semafor = semCreate("/semaphore",1);
        semInfo(semafor);
        semClose(semafor);
        break;
    
    case 'r': //usuń semafor
        semRemove("/semaphore");
        break;

    case 'p': //opuść semafor
        semafor = semOpen("/semaphore");
        semP(semafor);
        semInfo(semafor);
        semClose(semafor);
        break;

    case 'v': //podnieś semafor
        semafor = semOpen("/semaphore");
        semV(semafor);
        semInfo(semafor);
        semClose(semafor);
        break;

    case 'q': //wypisz informacje o semaforze
        semafor = semOpen("/semaphore");
        semInfo(semafor);
        semClose(semafor);
        break;
    
    default: //zły znak
        leave("invalid key")
    }

    return 0;
}