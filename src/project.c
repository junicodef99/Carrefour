#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h> 
#include <errno.h>

#define MUTEX 21

typedef struct Voie {
	int num;
	int sem_num[6];
	int crois[6];
} Voie;

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
int sem_id;
struct sembuf sem_oper ;  /* Operation P & V */

Voie voies[12] = {	{1,{0,1,2,3,4,5},{11,7,10,4,7,8}},
							{2,{6,7,8,9},{11,7,4,5}},
							{3,{-1},{-1}},
							{4,{8,10,3,11,12,13},{2,10,1,7,10,11}},
							{5,{9,14,15,16},{2,10,7,8}},
							{6,{-1},{-1}},
							{7,{15,4,11,17,1,7},{5,1,4,10,1,2}},
							{8,{16,5,18,19},{5,1,10,11}},
							{9,{-1},{-1}},
							{10,{18,12,17,2,10,14},{8,4,7,1,4,5}},
							{11,{19,13,0,6},{8,4,1,2}},
							{12,{-1},{-1}}	};
							
void P(int semnum)
{
	//printf("Blocage semaphore %d\n",semnum);
	sem_oper.sem_num = semnum-1;	// Numéro du sémaphore
	sem_oper.sem_op = -1 ;	// Opération (P(s))
	sem_oper.sem_flg = 0 ;	// NULL

	semop(sem_id,&sem_oper,1);
}

void V(int semnum)
{
	//printf("Deblocage semaphore %d\n",semnum);
	sem_oper.sem_num = semnum-1;	// Numéro du sémaphore
	sem_oper.sem_op = 1 ;	// Opération (V(s))
	sem_oper.sem_flg = 0 ;	// NULL

	semop(sem_id,&sem_oper,1);
}

int initsem() 
{
   sem_id = semget(IPC_PRIVATE, 16, IPC_CREAT | IPC_EXCL | 0666);
    
   union semun {
			int val;
			struct semid_ds *stat;
			ushort *array;
	} ctl_arg;

	ushort array[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};	// 20 => MUTEX
	ctl_arg.array = array;
	semctl(sem_id, 0, SETALL, ctl_arg);

	return (sem_id);
}

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

	tailleReq = sizeof(trequete) - sizeof(long);
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
	semctl(sem_id, 0, IPC_RMID, NULL);
	exit(0);
}

main(int argc,char* argv[])
{
	int nbVoitures;
	key_t cle;

	initsem();
	
	if (argc-1!=1) {
		fprintf(stderr,"Appel : %s <nb voitures>\n",argv[0]);
		exit(2);
	}
	nbVoitures = atoi(argv[1]);
	if ((cle = ftok(argv[0],'0')) == -1)
		erreurFin("Pb ftok");
	if ((msgid = msgget(cle,IPC_CREAT  | IPC_EXCL | 0600)) == -1)
		erreurFin("Pb msgget");
	forkn(nbVoitures,voiture);
	signal(SIGINT,traitantSIGINT);

	serveur();

	while (1) {
		pid_t done = wait();
		if (done == -1)
		if (errno == ECHILD) break;
	}

	msgctl(msgid, IPC_RMID, NULL);
	semctl(sem_id, 0, IPC_RMID, NULL);
}
