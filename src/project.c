#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/sem.h> 
#include <errno.h>
#include <string.h>
#include "structs.h"

int sem_id ;
struct sembuf sem_oper_P ;
struct sembuf sem_oper_V ; 

void P(int semnum) 
{
	sem_oper_P.sem_num = semnum;
	sem_oper_P.sem_op  = -1 ;
	sem_oper_P.sem_flg = 0 ;
	semop(sem_id, &sem_oper_P, 1); 
}

void V(int semnum)
{
	sem_oper_V.sem_num = semnum;
	sem_oper_V.sem_op  = 1 ;
	sem_oper_V.sem_flg  = 0 ;
 	semop(sem_id, &sem_oper_V, 1); 
}


int main(int argc, char * argv[])
{
	
	int pid, num;
	
	sem_id=semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
	semctl(sem_id, 0, SETVAL, 1);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	exit(0);
}
