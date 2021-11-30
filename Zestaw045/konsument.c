/* ==============================================================
 * plik konsument.c
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
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//flagi funkcji open
#define RESET_TAGS 01101
#define PERMISSIONS 0664
#define WRTIE_AND_APPEND 02001
#define READ_ONLY 00

/* ------------------------------------------------------------------------------- 
 * Definicje funkcji
 * -------------------------------------------------------------------------------  
 */

/* Funckja konsumująca pakiet danych i wysyłająca go do pliku.*/
void konsumuj(void);

/* Funkcja zamykająca deskryptory przy wyjściu z funkcji.*/
void sprzatanie(void);

/* Funckja wypisująca status konsumenta.
 *
 * param: *message - wczytany/zapisany pakiet danych
 * param: bytes - ilość bajtów wczytanych/zapisanych
 */
void status(char *message, int bytes);

/* ------------------------------------------------------------------------------- 
 * Makrodefinicje
 * -------------------------------------------------------------------------------  
 */

//makro do wyjścia z procesu
#define leave(message)       \
    {                        \
        perror(message);     \
        _exit(EXIT_FAILURE); \
    }

//makro do wylosowania liczby 
#define random (rand()%1000000)+1

/* ------------------------------------------------------------------------------- 
 * Zmienne globalne 
 * -------------------------------------------------------------------------------  
 */

extern int errno; //wykorzystywany do manualnego ustawiania errno

int pipe_dsc = 0, //deskryptor potoku
    file = 0, //deskryptor pliku
    pack_size; //ilość bajtów w przesłąnym pakiecie

int main(int argc, char *argv[]) {

    //ustawianie funkcji przy wywołaniu exit
    if(atexit(sprzatanie) == -1) {
        perror("[k] atexit error");
        _exit(EXIT_FAILURE);
    }

    //sprawdzanie czy przekazano wszystkie argumenty
    if (argc != 3) {
        errno = INVALID_ARGUMENTS;
        leave("[k] argument error")
    }

    //otwórz plik, jak go nie ma to go utwórz, jak jest to go wyzeruj
    if ((file = open(argv[2], RESET_TAGS, PERMISSIONS)) == -1)
        leave("[k] open error while reseting file")

    close(file); //zamknij deskryptor

    //otwó©z plik do pisania i dodawania nowych rzeczy
    if ((file = open(argv[2],WRTIE_AND_APPEND)) == -1)
        leave("[k] file open error")

    //otwórz potok do czytania
    if ((pipe_dsc = open(argv[1],READ_ONLY)) == -1)
        leave("[k] named pipe open error")

    srand(time(NULL)); //ustawia ziarno do losowania  

    //dopóki ilość bajtów w pakiecie jest większa od 0 konsumuj
    do {

            usleep(random);

            konsumuj();


        } while(pack_size > 0);
    
    _exit(EXIT_SUCCESS); //wyjdź

}

void konsumuj(void)
{   
        //alokacja bufora na pakiet
        char *bufor = (char*)calloc(8, sizeof(char));

        //alokacja bufora na liczbę bajtów
        char *bytes_bufor = (char*)calloc(2,sizeof(char));

        int bytes;

        if (bufor == NULL || bytes_bufor == NULL)
            leave("[k] bufor error")

        //wczytaj liczbe bajtów do wczytania
        if ((bytes = read(pipe_dsc, bytes_bufor, 1)) == -1)
            leave("[k] read pipe error 1")

        bytes = *bytes_bufor - '0'; //zapisz liczbę bajtów w zmiennej

        free(bytes_bufor); //zwolnij miejsce na bufor bajtów

        //wczytaj pakiet danych
        if ((bytes = read(pipe_dsc, bufor, bytes)) == -1)
            leave("[k] read pipe error 2")

        //zapisz pakiet danych w pliku
        if ((bytes = write(file, bufor, bytes)) == -1)
            leave("[k] write file error")

        pack_size = bytes; //przekaż informacje o zapisanych bajtach
        
        status(bufor,bytes); //wypisz status

        free(bufor); //zwolnij bufor

}

void status(char *message, int bytes) {

    if (bytes > 0) {
        printf("Konsument pobiera (%d B):\n",bytes);
        printf("%s\n",message);
        printf("--------------------------\n");
    }
    
}

void sprzatanie(void){
    printf("Deksryptory wyczyszczone!\n");
    if (pipe_dsc > 0)
        close(pipe_dsc);
    if (file > 0)
        close(file);
}