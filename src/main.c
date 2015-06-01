#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <signal.h>
#include <errno.h>
#include "project.h"
#include "main.h"
#include "interface.h"
#include "voiture.h"

int tailleReq = sizeof(Requete) - sizeof(long);
int tailleRep = sizeof(Reponse);

int msgid;
int sem_id;

main(int argc,char* argv[])
{
	key_t cle;

	if (argc-1 == 0) {
		printf("Syntaxe : ""./project Nb1 Nb2"" OR ""./project Nb1""\n");
		exit(-1);
	}
	
	initsem();

	file = fopen("./test.txt", "w");
	fclose(file);

	if ((cle = ftok(argv[0],'0')) == -1)
	erreurFin("Pb ftok");
	if ((msgid = msgget(cle,IPC_CREAT  | IPC_EXCL | 0600)) == -1)
	erreurFin("Pb msgget");

	signal(SIGINT,traitantSIGINT);

	if (argc-1 == 1) {
		premiere_ligne(atoi(argv[1]));
		forkn(atoi(argv[1]), NULL, voiture);
	} else {
		premiere_ligne(argc-1);
		forkn(argc-1, argv, voiture);
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

void erreurFin(const char* msg){ perror(msg); exit(1); }

/*------------- CODE GESTION PROCESSUS -----------------------*/

/* fonction permettant de creer nbFils qui executent la meme fonction */
void forkn(int nbFils, char *voies[], void (*fonction)())
{
	int i;
	
	if (voies == NULL) {
		for (i=0;i<nbFils;i++)
			if (fork()==0) {
				(*fonction) (i, -1);
				exit(0);
			}
	} else {
		for (i=0;i<nbFils;i++)
			if (fork()==0) {
				(*fonction) (i, atoi(voies[i+1]));
				exit(0);
			}
	}
}

void traitantSIGINT(int s)
{
	msgctl(msgid,IPC_RMID,NULL);
	semctl(sem_id, 0, IPC_RMID, NULL);
	exit(0);
}

void premiere_ligne(int num)
{
	int i;
	for (i = 0 ; i < num ; i++) {
		snprintf(buffer, sizeof(buffer), "     %s  %d     ", "Voiture", i);
		message(0, buffer);
	}
	message(0, "\n\n");
}
