/* ==============================================================
 * plik msg.h
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 11.05.2021
 * ostatnia modyfikacja: 14.05.2021
 * ==============================================================
 */

#ifndef MSG_h

#define MSG_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define MAX_SIZE 8192

#define PERMISSIONS 0644

//komunikat
typedef struct {
    int priority;
    char at[MAX_SIZE];
} komunikat;

/* Tworzy kolejke komunikatów i ją otwiera
 *
 * :member name: - nazwa kolejki 
 * 
 * :return: - deskryptor kolejki
 */
int msgCreate(const char *name);

/* Otwiera kolejka komunikatów
 *
 * :member name: - nazwa kolejki
 * 
 * :return: - deskryptor kolejki
 */
int msgOpen(const char *name);

/* Zamknij kolejke komunikatów
 *
 * :member msgid: - deskryptor kolejki
 */
void msgClose(int msgid);

/* Usuń kolejke komunikatów
 *
 * :member name: - nazwa kolejki
 */
void msgRm(const char *name);

/* Wysyła komunikat do kolejki
 *
 * :member msgid: - deskryptor kolejki
 * :member msg: - komunikat
 */
void msgSend(int msgid, komunikat *msg);

/* Odbiera komunikat do kolejki
 *
 * :member msgid: - deskryptor kolejki
 * :member msg: - komunikat
 */
void msgRecv(int msgid, komunikat *msg);

/* Wypisuje informacje o kolejce komunikatów
 *
 * :member msgid: - deskryptor kolejki
 */
void msgInfo(int msgid);

#endif