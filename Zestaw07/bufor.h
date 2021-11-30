/* ==============================================================
 * plik prodkons.h
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 3.05.2021
 * ostatnia modyfikacja: 10.05.2021
 * ==============================================================
 */

#ifndef PRODKONS_H

#define PRODKONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define ROZMIAR 10 

//typ towar
typedef struct {

    char at[9];

}towar;

//bufor cykliczny
typedef struct {

    int size;
    int beg;
    int end;
    towar buf[ROZMIAR];

} cykliczny;

/* Ustaw bufor do użytkowania
 *
 * :member bufor: - wskaźnik do bufora cyklicznego
 * :member s: - rozmiar bufora
 */
void bufInit(cykliczny *bufor, int s);

/* Pobierz towar z początku bufora
 *
 * :member bufor: - wskaźnik do bufora cyklicznego
 * :returns: - pobrany towar
 */
towar pop(cykliczny *bufor);

/* Wpisz towar na koniec bufora
 * 
 * :member bufor: - wskaźnik do bufora cyklicznego
 * :member t: - towar do wpisana
 * :returns: - 0 jeżeli operacja się powiedzie, -1 jak się nie uda
 */
int pushBack(cykliczny *bufor, towar t);

#endif