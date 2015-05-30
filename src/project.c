#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

typedef struct Voiture {
	int numero;
	int voie;
	int position;
} Voiture;

typedef struct {
	long  type;
	pid_t pidEmetteur;
	Voiture v;
} trequete;

typedef struct {
	long  type;
	Voiture v;
} treponse;

int msgid;

void initRand()
{
	srand(getpid());
}

void erreurFin(const char* msg){ perror(msg); exit(1); }

/*---------------- CODE CLIENT -----------------------*/

void constructionRequete(trequete *preq, Voiture *v)
{
	preq->pidEmetteur = getpid();
	preq->v = *v;
}

void affichageRequete(trequete *preq)
{
	printf("voiture %d, processus %d, voie %d\n", preq->v.numero, preq->pidEmetteur, preq->v.voie);
}

void affichageReponse(trequete *preq,treponse *prep)
{

}

void voiture(int numero)
{
	Voiture v;
	trequete req;
	treponse rep;
	int tailleReq, tailleRep;
	
	initRand();
	v.numero = numero;
	v.voie = rand()%12+1;

	tailleReq = sizeof(trequete) - sizeof(long);

//  while (1) {
	constructionRequete(&req, &v);
	affichageRequete(&req);
	req.type = 1;
	msgsnd(msgid,&req,tailleReq,0);
	tailleRep = sizeof(int);
/*
    break;
	msgrcv(msgid,&rep,tailleRep,getpid(),0);
	affichageReponse(&req,&rep);
	break;
	}
*/
}

/*---------------- CODE SERVEUR -----------------------*/

void constructionReponse(trequete *preq,treponse *prep)
{
	if (preq->type == 1) printf("Voiture %d proc %d arrive voie %d\n", preq->v.numero, preq->pidEmetteur, preq->v.voie);
	else if (preq->type == 2) printf("Voiture %d proc %d demande a passer intersection x voie %d\n", preq->v.numero, preq->pidEmetteur, preq->v.voie);
}

void serveur()
{
	trequete req;
	treponse rep;
	int  tailleReq,tailleRep;

	tailleReq    = sizeof(trequete) - sizeof(long);
	initRand();

	while (1) {
		msgrcv(msgid,&req,tailleReq,0,0);
		constructionReponse(&req,&rep);
/*
		break;
		rep.type = req.pidEmetteur;
		tailleRep = sizeof(int);
		msgsnd(msgid,&rep,tailleRep,0);
*/
	}
}

/*------------- CODE GESTION PROCESSUS -----------------------*/

/* fonction permettant de creer nbFils qui executent la meme fonction */
void forkn(int nbFils, void (*pauBoulot)())
{
	int i;
	for (i=0;i<nbFils;i++)
	if (fork()==0) {
		(*pauBoulot) (i);
		exit(0);
	}
}

void traitantSIGINT(int s)
{
	msgctl(msgid,IPC_RMID,NULL);
	exit(0);
}

main(int argc,char* argv[])
{
	int nbVoitures;
	key_t cle;

	if (argc-1!=1) {
		fprintf(stderr,"Appel : %s <nb voitures>\n",argv[0]);
		exit(2);
	}
	nbVoitures = atoi(argv[1]);
	if ((cle = ftok(argv[0],'0')) == -1)
		erreurFin("Pb ftok");
	if ((msgid = msgget(cle,IPC_CREAT  | IPC_EXCL | 0600)) == -1)
		erreurFin("Pb msgget 1");
	forkn(nbVoitures,voiture);
	signal(SIGINT,traitantSIGINT);

	serveur();

	while (1) {
		pid_t done = wait();
		if (done == -1)
		if (errno == ECHILD) break;
	}

	msgctl(msgid, IPC_RMID, NULL);
}
