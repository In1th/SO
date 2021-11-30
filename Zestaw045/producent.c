/* ==============================================================
 * plik producent.c
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

/* Funckja produkująca pakiet danych i wysyłająca go na potok.*/
void produkuj(void);

/* Funkcja zamykająca deskryptory przy wyjściu z funkcji.*/
void sprzatanie(void);

/* Funckja wypisująca status producenta.
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
        perror("[p] atexit error");
        _exit(EXIT_FAILURE);
    }

    //sprawdzanie czy przekazano wszystkie argumenty
    if (argc != 3) {
        errno = INVALID_ARGUMENTS;
        leave("[p] argument error")
    }

    //otwórz plik do czytania
    if ((file = open(argv[2],READ_ONLY)) == -1)
        leave("[p] file open error")

    //otwórz potok do zapisu
    if ((pipe_dsc = open(argv[1],02001)) == -1)
        leave("[p] named pipe open error")

    srand(time(NULL)); //ustawia ziarno do losowania 

    //dopóki ilość bajtów w pakiecie jest większa od 0 produkuj
    do {

            usleep(random);

            produkuj();

        } while(pack_size > 0);
    
    _exit(EXIT_SUCCESS); //wyjdź

}

void produkuj(void)
{

        //alokacja bufora
        char *bufor = (char*)calloc(8, sizeof(char));

        if (bufor == NULL)
            leave("[p] bufor error")

        int bytes, count = 0;

        //dynamiczne pakiety danych jak w prodkons.c
        do
        {
            char *buf = (char*)calloc(2, sizeof(char));
            if ((bytes = read(file, buf, 1)) == -1)
                leave("[p] read file error")

                if (bytes != 0)
                {
                    count++;
                    *(bufor + count - 1) = *buf;
                }
            free(buf);
        } while (bytes == 1 && count < 8 && *(bufor + count - 1) != ' ' && *(bufor + count - 1) != '\n');

        //wstawienie liczby wczytanych bajtów do char
        char count_char = count + '0';

        //wysłanie liczby wczytanych bajtów na potok dla konsumenta
        if ((bytes = write(pipe_dsc, &count_char, 1)) == -1)
            leave("[p] pipe write error")

        //wysłanie pakietu na potok
        if ((bytes = write(pipe_dsc, bufor, count)) == -1)
            leave("[p] pipe write error")

        pack_size = bytes; //przekazanie liczby bajtów w pakiecie

        status(bufor,bytes); //wypisanie statusu

        free(bufor); //zwolnij pamięć na bufor

}

void status(char *message, int bytes) {

    if (bytes > 0) {
        printf("Producent wysyla (%d B):\n",bytes);
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