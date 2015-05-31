#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "sem.h"

//struct sembuf sem_oper ;  /* Operation P & V */

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
