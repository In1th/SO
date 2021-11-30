/* ==============================================================
 * plik wysylaj.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 30.03.2021
 * ostatnia modyfikacja: 30.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//makro do wyjścia z procesu
#define leave(message) {perror(message); exit(EXIT_FAILURE);}

extern int errno;

int main(int argc, const char *argv[]) {

    if (argc != 3) {
        errno = INVALID_ARGUMENTS;
        leave("argument error")
    }

    int pid;

    switch (pid = fork())
    {
    case -1:
        leave("fork error");
        break;
    
    case 0:
        execl("./obsluga.x","./obsluga.x",argv[1],argv[2],NULL);
        leave("execl error")
        break;

    default:
        sleep(1);
        kill(pid,atoi(argv[2]));
        break;
    }


    return 0;
}