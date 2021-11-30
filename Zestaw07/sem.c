/* ==============================================================
 * plik sem.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 16.04.2021
 * ostatnia modyfikacja: 18.04.2021
 * ==============================================================
 */

#include "sem.h"

//makro do wyjścia z procesu
#define leave(message)       \
    {                        \
        perror(message);     \
        exit(EXIT_FAILURE); \
    }

semid semCreate(const char *name, int val) {
    printf("Tworzę semafor o nazwie %s\n",name);
    semid semafor;

    if((semafor = sem_open(name,O_CREAT | O_EXCL,0644,val)) == SEM_FAILED)
        leave("utworzenie semaforu")

    return semafor;
}

void semRemove(const char *name) {
    printf("Usuwam semafor o nazwie %s\n",name);

    if (sem_unlink(name) == -1)
        leave("usuwanie semaforu")
}

semid semOpen(const char *name) {
    printf("Otwieram semafor o nazwie %s\n",name);
    semid semafor;

    if ((semafor = sem_open(name,0)) == SEM_FAILED)
        leave("otwieranie semaforu")

    return semafor;
}

void semClose(semid sem) {
    printf("Zamykam semafor\n");

    if(sem_close(sem) == -1)
        leave("zamykanie semaforu")
}

void semP(semid sem) {
    printf("Opuszczam semafor\n");

    if (sem_wait(sem) == -1)
        leave("opuszczanie semafora")
}

void semV(semid sem) {
    printf("Podniesienie semafora\n");

    if (sem_post(sem) == -1)
        leave("podnoszenie semafora")
}

void semInfo(semid sem) {
    
    int val;

    if (sem_getvalue(sem,&val) == -1)
        leave("informacje semafora")

    printf("Semafor ma wartość %d\n",val);
}