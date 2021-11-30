/* ==============================================================
 * plik zombi.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 23.03.2021
 * ostatnia modyfikacja: 23.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "procinfo.h"

//makro do wyjścia z procesu
#define leave(message) {perror(message); return -1;}

int main(int argc, char *argv[]) {

	int PID = fork();


	if (PID == -1)
		leave("fork error")


	else if (PID == 0)
		return 0;
	
	else 
		printf("Stworzyłem proces zombie o PID:  %i\n", PID);
	

	return 0;
}