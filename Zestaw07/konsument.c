/* ==============================================================
 * plik konsument.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 5.05.2021
 * ostatnia modyfikacja: 10.05.2021
 * ==============================================================
 */

#define _XOPEN_SOURCE 500
#include <time.h>
#include <unistd.h>

#include "bufor.h"
#include "sem.h"
#include "shmem.h"

//funkcje atexit
void zamknij_obiekt(void);
void zamknij_producenta(void);
void zamknij_konsumenta(void);
void zamknij_plik(void);

//funkcja wykonująca zadania konsumenta
void konsumuj(void);

//funkcja wypisująca status
void status(towar t);

//flagi funkcji open
#define RESET_TAGS 01101
#define PERMISSIONS 0664
#define WRTIE_AND_APPEND 02001

//makro do wyjścia z procesu
#define leave(message)      \
    do                      \
    {                       \
        perror(message);    \
        exit(EXIT_FAILURE); \
    } while (0)

//makro wypisujące rodzaj procesu
#define sig printf("[K] ")

/* makro do wylosowania liczby mikrosekund do przespania
 *
 * przedział od 1 us do 1s
 */
#define random (rand() % 1000000) + 1

const char *obiekt = "/obiekt";
const char *prod = "/prod";
const char *kons = "/kons";
const char *schowek = "schowek.txt";

semid semafory[] = {NULL, NULL};
cykliczny *bufor = NULL;
int deskryptor = 0,
    plik = 0;

int main(int argc, char *argv[])
{

    srand(time(NULL)); //ustaw ziarno

    //otwórz obiekt pamięci dzielonej
    sig;
    if ((deskryptor = shmOpen(obiekt)) == -1)
        leave("[k] shm open error");

    //zmapuj obiekt pamięci dzielonej
    sig;
    if ((bufor = shmMap(deskryptor, sizeof(cykliczny))) == MAP_FAILED)
    {
        close(deskryptor);
        leave("[k] shm map error");
    }

    //ustaw atexit dla obiektu pamięci dzielonej
    if (atexit(zamknij_obiekt) == -1)
    {
        shmClose(bufor, deskryptor, sizeof(cykliczny));
        leave("[k] atexit shm error");
    }

    //otwórz semafor producenta
    sig;
    semafory[0] = semOpen(prod);

    //ustaw atexit semafora producenta
    if (atexit(zamknij_producenta) == -1)
    {
        semClose(semafory[0]);
        leave("[k] atexit prod error");
    }

    //otwórz semafor konsumenta
    sig;
    semafory[1] = semOpen(kons);

    //ustaw atexit semafora konsumenta
    if (atexit(zamknij_konsumenta) == -1)
    {
        semClose(semafory[1]);
        leave("[k] atexit kons error");
    }

    //otwórz plik, jak go nie ma to go utwórz, jak jest to go wyzeruj
    if ((plik = open(schowek, RESET_TAGS, PERMISSIONS)) == -1)
        leave("[k] open error while reseting file");

    close(plik); //zamknij deskryptor

    //otwórz plik do pisania i dodawania nowych rzeczy
    if ((plik = open(schowek, WRTIE_AND_APPEND)) == -1)
        leave("[k] file open error");

    //ustaw atexit dla pliku
    if (atexit(zamknij_plik) == -1)
    {
        close(plik);
        leave("[k] atexit file error");
    }

    printf("[k] Konfiguracja zakończona!\n");

    konsumuj();

    exit(EXIT_SUCCESS);
}

void zamknij_obiekt(void)
{
    shmClose(bufor, deskryptor, sizeof(cykliczny));
}

void zamknij_producenta(void)
{
    semClose(semafory[0]);
}

void zamknij_konsumenta(void)
{
    semClose(semafory[1]);
}

void zamknij_plik(void)
{
    close(plik);
}

void konsumuj(void)
{

    int bytes;

    towar t;

    do
    {

        //opusc swój semafor
        sig;
        semP(semafory[1]);

        usleep(random);

        //strefa krytyczna
        t = pop(bufor);

        if (strlen(t.at) > 0)
        {
            status(t);

            //zapisz pakiet danych w pliku
            if ((bytes = write(plik, t.at, strlen(t.at))) == -1)
                leave("[k] write file error");
        }

        //podnies prod
        sig;
        semV(semafory[0]);

    } while (t.at[0] != '\0');
}

void status(towar t)
{
    printf("[K] towar przesyłany -------\n");
    printf("%s\n", t.at);
    printf("(%ldB)------------------------\n", strlen(t.at));
}