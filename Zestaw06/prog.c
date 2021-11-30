/* ==============================================================
 * plik prog.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 24.04.2021
 * ostatnia modyfikacja: 27.04.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "sem.h"

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//flagi funkcji open
#define RESET_TAGS 01101
#define PERMISSIONS 0664
#define WRTIE_ONLY 01
#define READ_ONLY 00

//deklaracje funkcji
void sprzatanie(void);
void info(char mode, int count, semid semafor);
void sekcja_krytyczna(void);


//zmienne globalne
const char* name = "/sem";
const char* file = "numer.txt";
extern int errno;
semid semafor;

//makro do wyjścia z procesu
#define leave(message)       \
    {                        \
        perror(message);     \
        exit(EXIT_FAILURE); \
    }

/* makro do wylosowania liczby mikrosekund do przespania
 *
 * przedział od 1 us do 1s
 */
#define random (rand()%1000000)+1

int main(int argc, char *argv[]) {

    srand(time(NULL)); //ustawia ziarno do losowania

    //sprawdzanie czy przekazano wszystkie argumenty
    if (argc != 2) {
        errno = INVALID_ARGUMENTS;
        leave("argument number error")
    }

    int loop_number = atoi(argv[1]),
        count;

    semafor = semOpen(name); //otwieranie semafora

    //ustawianie funkcji przy wywołaniu exit
    if (atexit(sprzatanie) == -1)
        leave("atexit error")

    for (count = 0; count < loop_number; count++) {

        usleep(random);

        //opuszczanie semafora
        info('P',count + 1,semafor);
        semP(semafor);

        //sekcja krytyczna
        info('=',count+1,semafor);
        sekcja_krytyczna();

        //podnoszenie semafora
        semV(semafor);
        info('V',count+1,semafor);

    }

    exit(EXIT_SUCCESS);
}

void sprzatanie(void) {
    semClose(semafor);
}

void info(char mode, int count, semid semafor) {

    int pid = getpid();

    switch (mode)
    {
    case 'P':
        printf("(%i) przed opuszczeniem\n",pid);
        break;

    case '=':
        printf("\n(%i) sekcja krytyczna\n",pid);
        break;

    case 'V':
        printf("(%i) po podniesieniu\n",pid);
        break;
    
    default:
        break;
    }
    printf("(%i) ",pid);
    semInfo(semafor);
    printf("(%i) Numer sekcji krytycznej: %d\n",pid,count);
}

void sekcja_krytyczna(void) {

    int dsc, 
        bytes, 
        liczba;

    //otwórz plik
    if ((dsc = open(file,READ_ONLY)) == -1)
        leave("open error")

    char *bufor = (char*)calloc(8,sizeof(char));

    if (bufor == NULL)
        leave("calloc error")

    //wczytaj liczbe
    if ((bytes = read(dsc,bufor,8)) <= 0)
        leave("read error")

    close(dsc);

    printf("(%i) Liczba: %s\n",getpid(),bufor);

    liczba = atoi(bufor);

    free(bufor);

    liczba++;

    usleep(random);

    bufor =  (char*)calloc(bytes+1,sizeof(char));

    if (bufor == NULL)
        leave("calloc error")

    snprintf(bufor,bytes + 1,"%d", liczba);
    
    //otwórz plik
    if ((dsc = open(file,WRTIE_ONLY)) == -1)
        leave("open error")

    //wpisz liczbe
    if(write(dsc,bufor,bytes+1) == -1)
        leave("write error")

    close(dsc);
    free(bufor);

}