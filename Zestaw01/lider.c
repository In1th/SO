/* ==============================================================
 * plik lider.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 14.03.2021
 * ostatnia modyfikacja: 17.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "procinfo.h"

/* zmodyfikowana funkcja signum, dająca 1 dla liczb dodatnich
 * lub 0 jeżeli nie jest dodatnia
 */
#define sgn(x) (x>0)? (1) : (0)

//makro do wyjścia z procesu
#define leave(message) {perror(message); return -1;}

int main(int argc, const char* argv[]) {
	int i, j, deg = 0;

	procinfo(argv[0]);

	pid_t PID;
    int count = 0, status;

	for (i = 0; i < 3; i++){

        //wykonaj, jeżeli jest jakiś błąd
        if( (PID = fork()) == -1)
            leave("fork error")

		else if (PID == 0) {
			deg++;

			//ustaw, że ten proces ma 0 procesów potomnych
            count = 0;

            sleep(1);

            for (j = 0; j < deg; j++)
                printf(">");

            //ustaw PGID procesu potomnego
			if (setpgid(getpid(),0) == -1)
			    leave("setpgid")

			procinfo(argv[0]);
		}
		else {
			count++;
		}
	}

	//czekaj na wszystkie procesy potomne, jeżeli jakieś są
    while (count > 0) {
        status = wait(NULL);
        if (status == -1)
            leave("wait")
        else
            count -= sgn(status);
    }

    return 0;
}
