/* ==============================================================
 * plik grupa.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 30.03.2021
 * ostatnia modyfikacja: 31.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//makro do wyjścia z procesu
#define leave(message) do{perror(message); exit(EXIT_FAILURE);}while(0)

extern int errno;

const int iterations = 3;

int main(int argc, const char *argv[]) {

    if (argc != 3) {
        errno = INVALID_ARGUMENTS;
        leave("argument error");
    }

    int pid, i, count = iterations;

    int status;

    switch (pid = fork())
    {
    case -1:
        leave("fork error");
        break;

    case 0:
        if (setpgid(getpid(),0) == -1)
            leave("setpgid error");

        if(signal(atoi(argv[2]),SIG_IGN) == SIG_ERR)
            leave("signal error");

        for (i = 0; i < iterations; i++){
            switch (fork())
            {
            case -1:
                leave("fork error");
                break;
            
            case 0:
                execl("./obsluga.x","./obsluga.x",argv[1],argv[2],NULL);
                leave("exec error");
            }
        }

        while (count > 0) {
            pid = wait(&status);
            if (pid < 0)
                leave("wait");
            else if (pid != 0) {
                count--;
                printf("%d\n",status);
            }
        }

        break;
    
    default:
        sleep(2);
        if (kill(-pid,0) == 0)
            kill(-pid,atoi(argv[2]));
        else
            leave("kill error");
        wait(NULL);
        break;
    }

    return 0;
}