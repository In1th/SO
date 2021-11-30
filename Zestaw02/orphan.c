/* ==============================================================
 * plik orphan.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 23.03.2021
 * ostatnia modyfikacja: 23.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "procinfo.h"

//makro do wyjścia z procesu
#define leave(message) {perror(message); return -1;}

/*void koniec(void){
	procinfo("orphan");
	exit(0);
}*/

int main(int argc, char *argv[]) {

	int PID = fork();


	if (PID == -1)
		leave("fork error")


	else if (PID == 0) {
		procinfo(argv[0]);
		sleep(2);
		procinfo(argv[0]);
		exit(0);
		/*if (atexit(koniec) != 0)
			leave("atexit error")*/

	}
	
	else {
		sleep(1);
		printf("Koniec dzialania procesu macierzystego\n");
		return 0;
	}
	

	return 0;
}