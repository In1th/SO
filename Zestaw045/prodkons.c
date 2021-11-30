/* ==============================================================
 * plik prodkons.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 3.04.2021
 * ostatnia modyfikacja: 19.04.2021
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

/* Funckja wykonująca pracę producenta.
 * 
 * param: *magazyn - wskaźnik do nazwy pliku do czytania produktów
 */
void producent(char *magazyn);

/* Funckja wykonująca pracę konsumenta.
 * 
 * param: *schowek - wskaźnik do nazwy pliku do zapisywania produktów
 */
void konsument(char *schowek);

/* Funckja produkująca pakiet danych i wysyłająca go na potok.*/
void produkuj(void);

/* Funckja czytająca pakiet danych z potoku i zapisująca go w schowku.*/
void konsumuj(void);

/* Funckja wypisująca status producenta lub konsumenta.
 *
 * param: type - typ statusu (0 - producent, 1 - konsument)
 * param: *message - wczytany/zapisany pakiet danych
 * param: bytes - ilość bajtów wczytanych/zapisanych
 */
void status(int type, char *message, int bytes);

/* Funkcja zamykająca deskryptory przy wyjściu z funkcji.*/
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

//makro do zamkniecia deskryptora
#define close_dsc(dsc)  \
    {                   \
        close(fd[dsc]); \
        fd[dsc] = 0;    \
    }

/* makro do wylosowania liczby mikrosekund do przespania
 *
 * przedział od 1 us do 1s
 */
#define random (rand()%1000000)+1

/* ------------------------------------------------------------------------------- 
 * Zmienne globalne 
 * -------------------------------------------------------------------------------  
 */

extern int errno; //wykorzystywany do manualnego ustawiania errno

int fd[2] = {0, 0}; //do przechowywania deskryptorów (0 - do czytania, 1 - do pisania)

int size,
    pack_size; //zarządzanie ilością danych do wczytania/zapisania

int main(int argc, char *argv[])
{

    srand(time(NULL)); //ustawia ziarno do losowania

    //ustawianie funkcji przy wywołaniu exit
    if (atexit(sprzatanie) == -1)
    {
        perror("atexit error");
        exit(EXIT_FAILURE);
    }

    //sprawdzanie czy przekazano wszystkie argumenty
    if (argc != 3)
    {
        errno = INVALID_ARGUMENTS;
        leave("zla ilosc argumentow main")
    }

    //utworzenie potoku nienazwanego
    if (pipe(fd) == -1)
        leave("pipe error")

    //pobranie liczby bajtów z pliku do czytania
    struct stat *st = malloc(sizeof(struct stat));

    if (st == NULL)
        leave("struct stat")

    stat(argv[1], st);
    size = st->st_size;
    free(st);

    //utworzenie procesu potomnego
    switch (fork())
    {
    case -1: //błąd
        leave("fork error")
        break;
    case 0: //część dla konsumenta
        konsument(argv[2]);

        
    default: //część dla producenta
        producent(argv[1]);
    }

    return 0;
}

void producent(char *magazyn) {
    
    close_dsc(0) //zamknij niepotrzebny deskryptor potoku

        //otwórz plik do czytania
        if ((fd[0] = open(magazyn, READ_ONLY)) == -1)
            leave("open error while reading")

        //dopóki jeszcze są bajty do wczytania produkuj pakiety danych
        while(size > 0) {

            usleep(random);

            produkuj();

            size -= pack_size;

        }

        wait(NULL); //czekaj na konsumenta

        exit(EXIT_SUCCESS); //wyjdź

}

void konsument(char *schowek) {

    close_dsc(1) //zamknij niepotrzebny deskryptor potoku

        //otwórz plik, utworz go jeżeli go nie ma, a jak jest to wyzeruj jego zawartość
        if ((fd[1] = open(schowek, RESET_TAGS, PERMISSIONS)) == -1)
        leave("open error while reseting file")

        close_dsc(1) //zamknij plik

        //otwórz plik jeszcze raz, ale teraz do pisania i dodawania nowych rzeczy
        if ((fd[1] = open(schowek, WRTIE_AND_APPEND)) == -1)
            leave("open error while appending to file")

        //dopóki jeszcze są bajty do wczytania konsumuj pakiety danych
        while(size > 0) {

            usleep(random);

            konsumuj();

            size -= pack_size;

        } 

        exit(EXIT_SUCCESS); //wyjdź
}

void produkuj(void)
{
        
        //alokacja bufora na pakiet danych
        char *bufor = (char*)calloc(8, sizeof(char));

        if (bufor == NULL)
            leave("bufor error")

        int bytes, count = 0;

        /* dynamiczne pobieranie pakietu danych
         *
         * pętla pobiera znak po znaku do momentu kiedy:
         *  - napotka znak spacji lub znak nowej linii
         *  - wczyta 8 bajtów (maksymalna pojemność pakietu)
         *  - wczyta 0 bajtów (koniec znaków do wczytywania)
         */
        do
        {
            char *buf = (char*)calloc(2, sizeof(char));
            if ((bytes = read(fd[0], buf, 1)) == -1)
                leave("read file error")

                if (bytes != 0) //jeżeli coś zostało odczytane
                {
                    count++;
                    *(bufor + count - 1) = *buf;
                }
            free(buf);
        } while (bytes == 1 && count < 8 && *(bufor + count - 1) != ' ' && *(bufor + count - 1) != '\n');

        //wpisz wczytaną liczbę bajtów na potok
        if ((bytes = write(fd[1], bufor, count)) == -1)
            leave("pipe write error")

            pack_size = bytes; //przekaż informację o wczytanych bajtach

        status(0,bufor,bytes); //wypisz status

        free(bufor); //zwolnij miejsce buforu

}

void konsumuj(void)
{

        //alokacja bufora na pakiet danych
        char *bufor = (char*)calloc(8, sizeof(char));

        if (bufor == NULL)
            leave("bufor error")

        int bytes;

        //wczytaj z potoku pakiet danych
        if ((bytes = read(fd[0], bufor, 8)) == -1)
            leave("read pipe error")

        //zapisz pakiet danych w schowku
        if ((bytes = write(fd[1], bufor, bytes)) == -1)
            leave("write file error")


        if (bytes > 0) pack_size = bytes; //przekaż informację o wczytanych bajtach
    

        status(1,bufor,bytes); //wypisz status

        free(bufor); //zwolnij miejsce buforu

}

void status(int type, char *message, int bytes) {

    switch (type)
    {
    case 0:
        printf("Producent wysyla (%d B):\n",bytes);
        break;
    
    case 1:
        printf("Konsument pobiera (%d B):\n",bytes);
        break;
    }

    if (write(1,message,bytes) == -1) //równoznaczne z printf("%s\n",message);
        leave("write error")
    printf("\n--------------------------\n");

}

void sprzatanie(void)
{
    int i;

    for (i = 0; i < 2; i++)
        if (fd[i] > 0) //jeżeli deskryptor nie jest 0 to go zamknij
            close(fd[i]);

    printf("Deskryptory wyczyszczone!\n");
}