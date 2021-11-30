/* ==============================================================
 * plik sem.h
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 16.04.2021
 * ostatnia modyfikacja: 27.04.2021
 * ==============================================================
 */

#ifndef SEM_H

#define SEM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

//deklaracja typu semid
typedef sem_t *semid;

/* Funkcja tworząca i otwierająca semafor
 *
 * :member name: - wskaźnik do nazwy semafora
 * :member val: - wartość początkowa semafora
 * :returns: - wskaźnik na semafor
 */
semid semCreate(const char *name, int val);

/* Usuwa semafor
 *
 * :member name: - nazwa semafora
 */
void semRemove(const char *name);

/* Funkcja otwierająca semafor
 *
 * :member name: - nazwa semafora
 * :returns: - wskaźnik na semafor
 */
semid semOpen(const char *name);

/* Funkcja zamykająca semafor
 *
 *  :member sem: - wskaźnik na semafor
 */
void semClose(semid sem);

/* Funkcja opuszczająca semafor lub blokująca proces, jeżeli nie
 * może go opuścić.
 *
 * :member sem: - wskaźnik na semafor
 */
void semP(semid sem);

/* Funkcja podnosząca semafor i wznawiająca zablokowany proces
 *
 * :member sem: - wskaźnik na semafor
 */
void semV(semid sem);

/* Funkcja wypisująca informacje o semaforze
 *
 * :member sem: - wskaźnik na semafor
 */
void semInfo(semid sem);

#endif