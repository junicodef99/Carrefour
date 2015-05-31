#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include "project.h"
#include "interface.h"
#include "voiture.h"

int tailleReq = sizeof(trequete) - sizeof(long);
int tailleRep = sizeof(treponse);

int msgid;
int sem_id;

void erreurFin(const char* msg){ perror(msg); exit(1); }

/*------------- CODE GESTION PROCESSUS -----------------------*/

/* fonction permettant de creer nbFils qui executent la meme fonction */
void forkn(int nbFils, int voie, void (*pauBoulot)())
{
	int i;
	for (i=0;i<nbFils;i++)
		if (fork()==0) {
			(*pauBoulot) (i, -1);
			exit(0);
		}
}

void traitantSIGINT(int s)
{
	msgctl(msgid,IPC_RMID,NULL);
	semctl(sem_id, 0, IPC_RMID, NULL);
	exit(0);
}

main(int argc,char* argv[])
{
	int num;
	key_t cle;

	if (argc-1 == 0) {
		printf("Syntaxe : ""./project Nb1 Nb2"" OR ""./project Nb1""\n");
		exit(-1);
	} else {
		initsem();
		
		file = fopen("./test.txt", "w");
		fclose(file);
	
		if ((cle = ftok(argv[0],'0')) == -1)
			erreurFin("Pb ftok");
		if ((msgid = msgget(cle,IPC_CREAT  | IPC_EXCL | 0600)) == -1)
			erreurFin("Pb msgget");

		signal(SIGINT,traitantSIGINT);
	}
	
	if (argc-1 == 1) {
		for (num = 0 ; num < atoi(argv[1]) ; num++) {
			snprintf(buffer, sizeof(buffer), "     %s  %d     ", "Voiture", num);
			message(0, buffer);
		}
		message(0, "\n\n");

		forkn(atoi(argv[1]), -1, voiture);
	} else {
		for (num = 0 ; num < argc-1 ; num++) {
			snprintf(buffer, sizeof(buffer), "     %s  %d     ", "Voiture", num);
			message(0, buffer);
		}
		message(0, "\n");

		for (num = 0 ; num < argc-1 ; num++) {
			if (fork() == 0) {
					voiture(num, atoi(argv[num+1]));
					return;
			}
		}
	}
	
	serveur();

	while (1) {
		pid_t done = wait();
		if (done == -1)
			if (errno == ECHILD) break;
	}

	msgctl(msgid, IPC_RMID, NULL);
	semctl(sem_id, 0, IPC_RMID, NULL);
}
