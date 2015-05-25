#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <sys/sem.h> 
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>
#include "fonctions.h"


int shmid;

int main(int argc, char * argv[])
{
	sem_id=semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666);
	semctl(sem_id, 0, SETVAL, 1);
	
	shmid = shmget(IPC_PRIVATE, sizeof(Traffic), 0666);
	
	int pid, pidd, i, j, num;
	Traffic *trafficCarefour;
	
	
	
	
	num = atoi(argv[1]);
	
	trafficCarefour = malloc(4*sizeof(Traffic));
	trafficCarefour = (Traffic*)shmat(shmid, NULL, 0);
	initTrafficCarefour(trafficCarefour);
	
	//for(num =0; num<=4; num++)
	//{
		if (pid = fork() == 0)
		{
			//Echangeurs 
			printf("----------------- >> je suis le process echangeur de pid %d \n",getpid());
			
			for(i=0; i<atoi(argv[1]); i++)
			{
				
				if (pidd = fork() == 0)
				{
					
					//Véhicules
					
					Voiture voiture;
					initProprietesVehicule(&voiture);
					printf("**je suis le process voiture de pid %d mon echangeur est %d \n",getpid(),getppid());
					printf(" ----- Caracteristiques vehicule %d ------ \n",i+1);
					printf("--Num: %d  \n",voiture.num);
					printf("--Type: %s  \n",voiture.type);
					printf("--Vitesse: %d  \n",voiture.vitesse);
					printf("--Entree: echangeur %d, orientation %c  \n",voiture.entree.numEchangeur,voiture.entree.orientation);
					printf("--Sortie: echangeur %d, orientation %c  \n",voiture.sortie.numEchangeur,voiture.sortie.orientation);
					printf("--Itineraire: ");
					for(j=0; j<voiture.tailleItineraire; j++)
					{
						printf("ech: %d, orien %c ",voiture.itineraire[j].numEchangeur,voiture.itineraire[j].orientation);
					}
					printf("\n\n");
					while(voiture.tailleItineraire!=0)
					{
						while(voiture.positionSurEchangeur<0)
						{
							
							voiture.positionSurEchangeur+=voiture.vitesse;
							usleep(500000);
						}
						
						switch(voiture.entree.numEchangeur)
						{
							case 1:
								if(voiture.entree.orientation=='N' || voiture.entree.orientation=='S') 
									trafficCarefour[voiture.entree.numEchangeur-1].NS++;
								else if(voiture.entree.orientation=='E' || voiture.entree.orientation=='O') 
									trafficCarefour[voiture.entree.numEchangeur-1].EO++;
							break;
							/*
							case 2:
								if(voiture.entree.orientation=='N' || voiture.entree.orientation=='S') 
									trafficCarefour[voiture.entree.numEchangeur-1].NS++;
								else if(voiture.entree.orientation=='E' || voiture.entree.orientation=='O') 
									trafficCarefour[voiture.entree.numEchangeur-1].EO++;
							break;
							case 3:
								if(voiture.entree.orientation=='N' || voiture.entree.orientation=='S') 
									trafficCarefour[voiture.entree.numEchangeur-1].NS++;
								else if(voiture.entree.orientation=='E' || voiture.entree.orientation=='O') 
									trafficCarefour[voiture.entree.numEchangeur-1].EO++;
							break;
							case 4:
								if(voiture.entree.orientation=='N' || voiture.entree.orientation=='S') 
									trafficCarefour[voiture.entree.numEchangeur-1].NS++;
								else if(voiture.entree.orientation=='E' || voiture.entree.orientation=='O') 
									trafficCarefour[voiture.entree.numEchangeur-1].EO++;
							break;
							*/
						}
						voiture.tailleItineraire--;
						
					}
					printf("-**voiture num %d est morte**- \n",voiture.num);
					printf("\n\n");
					
					
					free(voiture.itineraire);
					exit(1);
				}
				
				//wait(NULL);
				sleep(1);
			}
			
			
			
			
			
			
			
			
			while(num > 0)
			{
				wait(NULL);
				num--;
			}
		}

	//}

	wait(NULL);
	fprintf(stdout,"%d - %d\n",trafficCarefour[0].NS,trafficCarefour[0].EO);
	
		
	exit(0);
}
