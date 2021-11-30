/* ==============================================================
 * plik prodkons.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 3.05.2021
 * ostatnia modyfikacja: 10.05.2021
 * ==============================================================
 */

#include "bufor.h"

extern int errno;

void bufInit(cykliczny *bufor, int s) {

    printf("(bufor) init\n");

    //dany rozmiar jest większy niż rozmiar maksymalny
    if (s > ROZMIAR) {
        errno = EINVAL;
        perror("bufor init error");
        exit(EXIT_FAILURE);
    }

    bufor -> size = s;
    bufor -> beg = 0;
    bufor -> end = 0;

    int i,j;

    //wyzeruj wszystkie elementy
    for (i = 0; i < bufor -> size; i++) {
        for (j = 0; j < 9; j++) bufor -> buf[i].at[j] = 0;
    }

}

towar pop(cykliczny *bufor) {
    
    towar output;

    printf("(bufor) pop\n");
    
    int i = 0;

    for (i = 0;i < 9;i++){
        output.at[i] = bufor -> buf[bufor -> beg].at[i];
        bufor -> buf[bufor -> beg].at[i] = 0;
    }

    bufor -> buf[bufor -> beg].at[0] = 0;

    bufor -> beg = (bufor -> beg + 1)%(bufor -> size);

    return output;
}

int pushBack(cykliczny *bufor, towar t) {

    printf("(bufor) push back\n");

    //sprawdź czy bufor nie jest pełny
    if (bufor -> beg == bufor -> end && strlen(bufor -> buf[bufor -> beg].at) != 0) {
        errno = ENOBUFS;
        return -1;
    }

    int i = 0;

    for(i = 0; i < 9; i++){
        bufor -> buf[bufor -> end].at[i] = t.at[i];
    }

    bufor -> end = (bufor -> end + 1)%(bufor -> size);

    return 0;
    
}