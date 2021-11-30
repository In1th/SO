/* ==============================================================
 * plik shmem.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 3.05.2021
 * ostatnia modyfikacja: 3.05.2021
 * ==============================================================
 */

#include <errno.h>
#include <string.h>

#include "shmem.h"

//makro do wyjścia z procesu
#define leave(message)         \
    do{                        \
        perror(message);       \
        exit(EXIT_FAILURE);    \
    } while(0)

extern int errno;

int main(int argc, char *argv[]) {

    //wypisz instrukcje
    if (argc == 1) {
        printf("Program %s przyjmuje takie flagi:\n",argv[0]);
        printf("c size - tworzy segment pamięci dzielonej o danej liczbie bajtów\n");
        printf("d - usuwa segment pamięci dzielonej\n");
        printf("r - wypisuje zawartość pamięci dzielonej\n");
        printf("w - kopiuje zawartość standardowego wejścia do pamięci\n");
        printf("i - wypisuje informacje o segmencie\n");
        exit(EXIT_SUCCESS);
    }
    
    int deskryptor, rozmiar;

    char *zawartosc;

    switch (argv[1][0])
    {
    case 'c': //utwórz obiekt
        
        if (argc < 2) {
            errno = EINVAL;
            leave("no size error");
        }
        
        if ((deskryptor = shmCreate("/segment",atoi(argv[2]))) == -1)
            leave("create error");

        if (close(deskryptor) == -1)
            leave("close error");

        break;

    case 'd': //usuń obiekt
        shmRm("/segment");
        break;

    case 'r': //czytaj obiekt

        if ((deskryptor = shmOpen("/segment")) == -1)
            leave("create error");

        if ((rozmiar = shmSize(deskryptor)) == -1)
            leave("size error");
        
        if ((zawartosc = (char *)shmMap(deskryptor,rozmiar)) == MAP_FAILED)
            leave("map error");

        printf("%s\n",zawartosc);

        shmClose(zawartosc,deskryptor,rozmiar);

        break;

    case 'w': //wczytaj coś do obiektu
        
        if ((deskryptor = shmOpen("/segment")) == -1)
            leave("create error");

        if ((rozmiar = shmSize(deskryptor)) == -1)
            leave("size error");

        if ((zawartosc = (char *)shmMap(deskryptor,rozmiar)) == MAP_FAILED)
            leave("map error");

        char *bufor = (char*)calloc(1,sizeof(char));

        if (scanf("%s",bufor) == EOF)
            leave("scanf error");

        if (strlen(bufor) > rozmiar) {
            errno = EINVAL;
            leave("input size too big");
        }

        int i = 0;

        while(*(bufor + i) != '\0') {
            *(zawartosc + i) = *(bufor + i);
            i++;
        }

        printf("%s\n",zawartosc);

        shmClose(zawartosc,deskryptor,rozmiar);

        free(bufor);

        break;

    case 'i': //wypisz informacje o obiekcie
        if ((deskryptor = shmOpen("/segment")) == -1)
            leave("create error");

        shmInfo(deskryptor);

        if (close(deskryptor) == -1)
            leave("close error");

        break;
    
    default: //zły klucz
        errno = EINVAL;
        leave("key error");
        break;
    }

    exit(EXIT_SUCCESS);

}
