#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/sem.h> 
#include <errno.h>
#include <string.h>

#define MAXPAUSE 2
//d
struct sembuf sem_oper ;  /* Operation P & V */

typedef struct Voie {
	int num;
	int sem_num[6];
	int crois[6];
} Voie;

typedef struct Voiture {
	int num;
	Voie *voie;
} Voiture;

int sem_id;
char buffer[1024];
char output[10240];
FILE *file;
Voiture voiture;
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
	sem_oper.sem_op  = -1 ;	// Opération (P(s))
	sem_oper.sem_flg = 0 ;	// NULL

	semop(sem_id,&sem_oper,1);
}

void V(int semnum)
{
	//printf("Deblocage semaphore %d\n",semnum);
	sem_oper.sem_num = semnum-1;	// Numéro du sémaphore
	sem_oper.sem_op  = 1 ;	// Opération (V(s))
	sem_oper.sem_flg  = 0 ;	// NULL

	semop(sem_id,&sem_oper,1);
}

void message(int i, char* s) {
   #define COLONNE 20
   int j, NbBlanc;
   file = fopen("./test.txt", "a");
   NbBlanc=i*COLONNE;
   for (j=0; j<NbBlanc; j++) {
		putchar(' ');
		fprintf(file, " ");
	}
   printf("%s",s);
   fprintf(file, "%s", s);
   fflush(stdout);
   fclose(file);
}

void run()
{
	int val, i;

	srand(time(NULL) ^ getpid());
	sleep(rand()%MAXPAUSE);
	
	for (i=0 ; i < 6 ; i++) {
		int c = voiture.voie->crois[i];
		if (c == -1) break;
		int s = voiture.voie->sem_num[i];
		val = semctl(sem_id, c-1, GETVAL, NULL);
		if (val == 0) {
			P(21);
			snprintf(buffer, sizeof(buffer), "Priorite a voie %d\n", c);
			message(voiture.num, buffer);
			V(21);
			P(s);
			P(21);
			snprintf(buffer, sizeof(buffer), "Repart voie %d\n", c);
			message(voiture.num, buffer);
			V(21);
		} else {
			P(s);
		}
		P(21);
		snprintf(buffer, sizeof(buffer), "Traverse voie %d\n", c);
		message(voiture.num, buffer);
		V(21);
		sleep(rand()%MAXPAUSE);
		P(21);
		snprintf(buffer, sizeof(buffer), "A traverse voie %d\n", c);
		message(voiture.num, buffer);
		V(21);
		V(s);
	}
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

////////////////////////////////////////////////////////////////////////////////////

void initvoiture(int num)
{
	int voie;
	srand(time(NULL) ^ getpid());
	
	voie = rand()%12+1;
	
	voiture.num = num;
	voiture.voie = &voies[voie-1];
	P(21);
	snprintf(buffer, sizeof(buffer), "Arrive voie %d\n", voiture.voie->num);
	message(voiture.num, buffer);
	V(21);
	run();
	P(21);
	snprintf(buffer, sizeof(buffer), "Sort voie %d\n", voiture.voie->num);
	message(voiture.num, buffer);
	V(21);
}

void initvoiture1(int num, int voie)
{
	voiture.num = num;
	voiture.voie = &voies[voie-1];
	P(21);
	snprintf(buffer, sizeof(buffer), "Arrive voie %d\n", voiture.voie->num);
	message(voiture.num, buffer);
	V(21);
	run();
	P(21);
	snprintf(buffer, sizeof(buffer), "Sort voie %d\n", voiture.voie->num);
	message(voiture.num, buffer);
	V(21);
}

int main(int argc, char* argv[])
{
	int num;
	int pid;

	file = fopen("./test.txt", "w");
	fclose(file);

	if (argc-1 == 1) {
	
		initsem();
		
		for (num = 0 ; num < atoi(argv[1]) ; num++) {
			snprintf(buffer, sizeof(buffer), "     %s  %d     ", "Voiture", num);
			message(0, buffer);
		}
		message(0, "\n\n");

		for (num = 0 ; num < atoi(argv[1]) ; num++) {
			if (pid = fork() == 0) {
				initvoiture(num);
				return;
			}
		}
		
	} else if (argc-1 >= 2) {
	
		initsem();
	
		for (num = 0 ; num < argc-1 ; num++) {
			snprintf(buffer, sizeof(buffer), "     %s  %d     ", "Voiture", num);
			message(0, buffer);
		}
		message(0, "\n");
	
		for (num = 0 ; num < argc-1 ; num++) {
			if (pid = fork() == 0) {
					initvoiture1(num, atoi(argv[num+1]));
					return;
			}
		}
		
	} else {

		printf("Syntaxe : ""./project Nb1 Nb2"" OR ""./project Nb1""\n");
		exit(-1);

	}
	while (1) {
		pid_t done = wait();
		if (done == -1)
			if (errno == ECHILD) break;
	}
	
	semctl(sem_id, 0, IPC_RMID, NULL);
 	
	exit(0);
}
