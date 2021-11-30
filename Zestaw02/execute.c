/* ==============================================================
 * plik execute.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 24.03.2021
 * ostatnia modyfikacja: 24.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

//makro do wyjścia z procesu
#define leave(message) {perror(message); return -1;}

int main(int argc, char *argv[]) {

	printf("Pierwszy komunikat\n");

	if (execl("./potomny.x","./potomny.x",NULL) == -1)
		leave("execl")

	printf("Drugi komunikat\n");


	return 0;
}