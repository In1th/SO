/* ==============================================================
 * plik shmem.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 3.05.2021
 * ostatnia modyfikacja: 3.05.2021
 * ==============================================================
 */

#include "shmem.h"


int shmCreate(const char* name, int size) {

    printf("(libshmem) Tworzenie obiektu\n");

    int dsc;

    if ((dsc = shm_open(name,O_RDWR | O_CREAT | O_EXCL,0644)) == -1)
        return -1;

    if (ftruncate(dsc,size) == -1)
        return -1;

    return dsc;

}

int shmOpen(const char* name) {

    printf("(libshmem) Otwieranie obiektu\n");

    int dsc;

    if ((dsc = shm_open(name,O_RDWR,0)) == -1)
        return -1;

    return dsc;
}

void shmRm(const char* name) {

    printf("(libshmem) Usuwanie obiektu\n");

    if(shm_unlink(name) == -1) {
        perror("unlink error");
        exit(EXIT_FAILURE);
    }

}

void* shmMap(int fd, int size) {

    printf("(libshmem) Mapowanie obiektu\n");

    void *ptr = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    return ptr;
}

void shmClose(void *ptr, int fd, int size) {

    printf("(libshmem) Zamykanie obiektu\n");

    if (munmap(ptr,size) == -1) {
        perror("munmap error");
        exit(EXIT_FAILURE);
    }

    if(close(fd) == -1) {
        perror("close error");
        exit(EXIT_FAILURE);
    }

}

int shmSize(int fd) {
    printf("(libshmem) Czytanie wielkości obiektu\n");

    struct stat st;

    if (fstat(fd,&st) == -1) {
        perror("fstat error");
        return -1;
    }

    return st.st_size;
}

void shmInfo(int fd) {
    struct stat st;

    if (fstat(fd,&st) == -1) {
        perror("fstat error");
        exit(EXIT_FAILURE);
    }

    printf("(libshmem) Informacje o obiekcie:\n");
    printf("Rozmiar: %ldB\n",st.st_size);
    printf("Wlasciciel: %i Grupa: %i\n",st.st_uid,st.st_gid);
    printf("Prawa dostepu: %o\n",st.st_mode);

}