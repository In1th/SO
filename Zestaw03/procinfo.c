/* ==============================================================
 * procinfo.c file
 * ==============================================================
 * author: Mateusz Kruk, group 7.
 * created: 02.03.2021
 * last modified: 14.03.2021
 * ==============================================================
 */

#include "procinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* --------------------------------------------------------------
 * Function: procinfo
 * Declaration: procinfo.h:
 * --------------------------------------------------------------
 * Prints out process name and UID, GID, PID, PPID and PGID identifiers
 *
 * name: process name
 */
int procinfo(const char* name){

	uid_t UID = getuid();
	gid_t GID = getgid();
	pid_t PID = getpid();
	pid_t PPID = getppid();
	pid_t PGID = getpgid(PID);

	printf("name = %s, UID = %i, GID = %i, PID = %i, PPID = %i, PGID = %i \n",name,UID,GID,PID,PPID,PGID);

	//there is nothing in here that could cause an error, so this function doesn't require any error handling
	return 0;

}
