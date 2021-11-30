/* ==============================================================
 * plik potomny.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 20.03.2021
 * ostatnia modyfikacja: 23.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "procinfo.h"

int main(int argc, const char* argv[]) {

	//wypisz ten proces
	procinfo(argv[0]);

	return 0;
}