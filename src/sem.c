#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "sem.h"

void P(int semnum)
{
	sem_oper.sem_num = semnum-1;	// Numéro du sémaphore
	sem_oper.sem_op = -1 ;	// Opération (P(s))
	sem_oper.sem_flg = 0 ;	// NULL

	semop(sem_id,&sem_oper,1);
}

void V(int semnum)
{
	sem_oper.sem_num = semnum-1;	// Numéro du sémaphore
	sem_oper.sem_op = 1 ;	// Opération (V(s))
	sem_oper.sem_flg = 0 ;	// NULL

	semop(sem_id,&sem_oper,1);
}

int initsem()
{
	sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);

	semctl(sem_id, 0, SETVAL, 1);

	return (sem_id);
}