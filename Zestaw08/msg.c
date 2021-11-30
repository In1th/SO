/* ==============================================================
 * plik msg.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 11.05.2021
 * ostatnia modyfikacja: 14.05.2021
 * ==============================================================
 */

#include "msg.h"

extern int errno;

int msgCreate(const char *name) {

    int dsc;

    printf("(MSGQUEUE) Tworzenie kolejki %s\n",name);

    if (*name != '/') {
        errno = EINVAL;
        perror("name without /");
        exit(EXIT_FAILURE);
    }



    if ((dsc = mq_open(name,O_RDWR | O_CREAT | O_EXCL,PERMISSIONS,NULL)) == -1) {
        perror("mq create error");
        exit(EXIT_FAILURE);
    }

    return dsc;

}

int msgOpen(const char *name) {

    int dsc;

    printf("(MSGQUEUE) Otwieranie kolejki %s\n",name);

    if (*name != '/') {
        errno = EINVAL;
        perror("name without /");
        exit(EXIT_FAILURE);
    }

    if ((dsc = mq_open(name,O_RDWR)) == -1) {
        perror("mq open error");
        exit(EXIT_FAILURE);
    }

    return dsc;

}

void msgClose(int msgid) {
    
    printf("(MSGQUEUE) Zamykanie kolejki\n");

    if (mq_close(msgid) == -1) {
        perror("mq close error");
        exit(EXIT_FAILURE);
    }

}

void msgRm(const char *name) {

    printf("(MSGQUEUE) Usuwanie kolejki %s\n",name);

    if (*name != '/') {
        errno = EINVAL;
        perror("name without /");
        exit(EXIT_FAILURE);
    }

    if (mq_unlink(name) == -1) {
        perror("mq unlink error");
        exit(EXIT_FAILURE);
    }

}

void msgSend(int msgid, komunikat *msg) {

    printf("(MSGQUEUE) Wysyłanie komunikatu\n");

    if (mq_send(msgid,msg -> at,MAX_SIZE,msg -> priority) == -1) {
        perror("mq send error");
        exit(EXIT_FAILURE);
    }

}

void msgRecv(int msgid, komunikat *msg) {

    printf("(MSGQUEUE) Pobieranie komunikatu\n");

    if (mq_receive(msgid,msg -> at,MAX_SIZE,&(msg -> priority)) == -1) {
        perror("mg receive error");
        exit(EXIT_FAILURE);
    }

}

void msgInfo(int msgid) {

    printf("(MSGQUEUE) Pobieranie informacji o kolejce\n");

    struct mq_attr atr;

    if (mq_getattr(msgid,&atr) == -1) {
        perror("mq getattr error");
        exit(EXIT_FAILURE);
    }

    printf("mg_flags: %ld\n",atr.mq_flags);
    printf("mq_maxmsg: %ld\n",atr.mq_maxmsg);
    printf("mq_msgsize: %ld\n",atr.mq_msgsize);
    printf("mq_curmsgs: %ld\n",atr.mq_curmsgs);

}
