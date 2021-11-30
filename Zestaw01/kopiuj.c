/* ==============================================================
 * plik kopiuj.c
 * ==============================================================
 * autor: Mateusz Kruk, grupa 7.
 * utworzony: 02.03.2021
 * ostatnia modyfikacja: 14.03.2021
 * ==============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//kod wartości errno dla niewłaściwych argumentóœ
#define INVALID_ARGUMENTS EINVAL

//flagi funkcji open
#define READ_ONLY O_RDONLY
#define WRITE_ONLY O_WRONLY
#define CREATE_IF_ABSENT O_CREAT

//kody permisji pliku, wymagane jeżeli korzysta się z flagi O_CREAT
#define USER_WRITE S_IWUSR
#define USER_READ S_IRUSR
#define GROUP_READ S_IRGRP
#define GROUP_WRITE S_IWGRP
#define OTHERS_READ S_IROTH

//makro do wyjścia z procesu
#define leave(message) {perror(message); return -1;}
#define close_and_leave(descriptor,message) { perror(message); close(descriptor); return -1;}

extern int errno;

int main(int narg, const char* argv[]){

	int fcopy(const char *source, const char *target); //prototyp fcopy

	//sprawdza czy main otrzymał dokładnie 2 argumenty
	if (narg < 3 || narg > 3){
		errno = INVALID_ARGUMENTS;
		printf("Got %d arguments instead of 2! Make sure you've entered two filenames! \n", (narg-1));

		leave(argv[0])
	}

	else 
		return fcopy(argv[1],argv[2]);
}

/* --------------------------------------------------------------
 * Funkcja: fcopy
 * --------------------------------------------------------------
 * Kopiuje zawartość pliku źródłowego do pliku docelowego wraz z 
 * informacją o ilości bajtów.
 *
 * *source: nazwa pliku źródłowego
 * *target: nazwa pliku docelowego
 */
int fcopy(const char *source, const char *target) {

	int descriptor;

	//otwórz plik źródłowy i sprawdź czy open nie zwrócił błędu
	if ((descriptor = open(source,READ_ONLY)) == -1)
		close_and_leave(descriptor,source)

	else {

		int status;

		//wczytaj ilość bajtów pliku
		struct stat *st = malloc(sizeof(struct stat));

		//alokacja pamięci się niepowiodła
		if (st == NULL)
			leave("struct stat")

		//wczytaj ilość bajtów pliku
		stat(source, st);
		int size = st -> st_size;
		free(st);

		char *bufor = malloc((size + 1) * sizeof(char));

		if (bufor == NULL)
			leave("bufor")


		//poprawne wczytanie napisu do buforu
		status = read(descriptor, bufor, size);
		*(bufor + size) = '\0';
		close(descriptor);

		if (status == -1){
			free(bufor);
			leave(source)
		}

		//otwórz plik docelowy lub go stwórz jeżeli nie ma i sprawdź czy open nie zwrócił błędu
		if ((descriptor = open(target, CREATE_IF_ABSENT | WRITE_ONLY, 0664)) == -1){
			free(bufor);
			close_and_leave(descriptor,target)
		}


		//sprawdź ilość cyfr w liczbie bajtów size
		int byte_size = snprintf( NULL, 0, "%d", size);
		char *info_string = malloc(byte_size + 10);

		if (info_string == NULL)
			leave("info string")


		//wpisuje napis do info_string według formatowania
		snprintf(info_string, byte_size + 10, "Size: %d B\n",size);

		//napisz w pliku docelowym jego ilość bajtów
		status = write(descriptor,info_string,byte_size+9);
		free(info_string);

		//przenieś zawartość buforu do pliku docelowego
		status = write(descriptor,bufor,size);
		close(descriptor);
		free(bufor);

		if (status == -1)
			leave(target)
		
	}
	return 0;
}