/* ==============================================================
 * plik shmem.h
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 3.05.2021
 * ostatnia modyfikacja: 3.05.2021
 * ==============================================================
 */

#ifndef SHMEM_H

#define SHMEM_H

#ifndef LIBS

#define LIBS

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#endif

/* Funkcja tworzy pamięć dzieloną o nazwie name i rozmiarze size.
 * Funkcja zwraca deskryptor obiektu pamięci dzielonej.
 */
int shmCreate(const char* name, int size);

/* Funkcja otwiera obiekt pamięci dzielonej i zwraca jego deskryptor.
 */
int shmOpen(const char* name);

/* Usuwa obiekt pamięci dzielonej o nazwie name.
 */
void shmRm(const char* name);

/* Odwzorowuje obiekt pamięci dzielonej o deskryptorze fd i rozmiarze size w przestrzeń adresową procesu.
 * Funkcja zwraca adres początku odwzorowanego obszaru pamięci dzielonej.
 */
void* shmMap(int fd, int size);

/* Usuwa odwzorowanie obiektu pamięci dzielonej i zamyka powiązany z nim deskryptor.
 */
void shmClose(void *ptr, int fd, int size);

/* Zwraca rozmiar pamięci dzielonej określonej deskryptorem fd.
 * Proszę użyć funkcji systemowej fstat.
 */
int shmSize(int fd);

/* Wypisuje informacje o pamięci dzielonej: prawa dostępu, rozmiar, właściciel, etc.
 */
void shmInfo(int fd);

#endif