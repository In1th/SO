/* ==============================================================
 * plik drzewko1.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 20.03.2021
 * ostatnia modyfikacja: 20.03.2021
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
#define sgn(x) (x > 0)? (1): (0)

//makro do wyjścia z procesu
#define leave(message) {perror(message); return -1;}

int main(int argc, const char* argv[]) {
	int i, j, deg = 0;

	//wypisz ten proces
	procinfo(argv[0]);

	pid_t PID;
	int count = 0, status;

	for (i = 0; i < 3; i++){

	    //wykonaj, jeżeli jest jakiś błąd
		if( (PID = fork()) == -1)
			leave("fork error")

		//część dla procesu potownego
		else if (PID == 0) {
            deg++;
            i = 2;

            //ustaw, że ten proces ma 0 procesów potomnych
			count = 0;

			sleep(1);

			for (j = 0; j < deg; j++)
			    printf(">");

			procinfo(argv[0]);
		}

		//część dla procesu macierzystego
        else
            count++;
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