#include <stdlib.h>
#include <stdio.h>
#include "fonctions.h"


int shmid, shmid2, shmid3;

int main(int argc, char * argv[])
{
	if(sem_id=semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0666) == -1)
	{
		printf("Impossible de creer la semaphore");
		exit(1);
	}
	semctl(sem_id, 0, SETVAL, 1);
	
	shmid = shmget(CLEMEM1, sizeof(Traffic), IPC_CREAT | 0666);
	shmid2 = shmget(CLEMEM2, sizeof(int), IPC_CREAT | 0666);
	shmid3 = shmget(CLEMEM3, sizeof(int), IPC_CREAT | 0666);
	
	int msgid;			
	key_t cle;		
	int longMSG ;	    
	RequeteVoiture requeteV;
	RequeteEchangeur requeteE;
	ReponseRequete reponseReq;
	/*	
	if ((cle = ftok(argv[0], 'u')) == -1) 
	{
		perror("Erreur de creation de la cle \n");
		exit(1);
	}
	*/
	if ((msgid = msgget(CLEMEMFILE1, 0750 | IPC_CREAT)) == -1) 
	{
		perror("Erreur de creation de la file de message\n");
		exit(1);
	}
	
	
	int pid, pidd, i, j, num = atoi(argv[1]);
	int *missionsAccomplies, *etatVehiculesInitialises;
	Traffic *trafficCarefour;	
	
	trafficCarefour = malloc(4*sizeof(Traffic));
	trafficCarefour = (Traffic*)shmat(shmid, NULL, 0);
	initTrafficCarefour(trafficCarefour);
	
	missionsAccomplies = (int*)shmat(shmid2, NULL, 0);
	*missionsAccomplies = 0;
	
	etatVehiculesInitialises = (int*)shmat(shmid3, NULL, 0);
	*etatVehiculesInitialises = 0;
	
	printf("##: %d\n",*etatVehiculesInitialises);
	if (atoi(argv[1]) < 1 || atoi(argv[1]) > 20)
	{
		fprintf(stderr,"**Vous de devez definir en parametre un nombre de vehicules**\n");
		exit(0);
	}
	//for(num =0; num<=4; num++)
	//{
		if(pid = fork() == 0)
		{
			//Echangeurs 
			printf("----------------- >> je suis le process echangeur de pid %d \n",getpid());
			
			for(i=0; i<atoi(argv[1]); i++)
			{
				
				if(pidd = fork() == 0)
				{
					
					//V�hicules
					
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
								//On s'assure d'abord que tous les v�hicules ont �t�s initialis�s avant de lancer l'annimation.	
							if(*etatVehiculesInitialises == 1)
							{
								voiture.positionSurEchangeur+=voiture.vitesse;
								usleep(500000);
							}
						}
						switch(voiture.entree.numEchangeur)
						{
							case 1:
								if(voiture.entree.orientation=='N' || voiture.entree.orientation=='S') 
								{
									P(0);
									trafficCarefour[voiture.entree.numEchangeur-1].NS++;
									V(0);
								}
								else if(voiture.entree.orientation=='E' || voiture.entree.orientation=='O') 
								{
									P(0);
									trafficCarefour[voiture.entree.numEchangeur-1].EO++;
									V(0);
								}
									//Construction et envoi de requete � l'Echangeur
								constructionRequeteVoiture(&requeteV, voiture.entree, voiture.type);
								if(msgsnd(msgid,&requeteV,sizeof(RequeteVoiture),0) == -1)
								{
									printf("Erreur d'envoi de la requete a l'echangeur %d \n", voiture.entree.numEchangeur);
									exit(1);
								}
									//On boucle tant qu'on ne re�oit pas de r�ponse de l'�changeur
								while (reponseReq.reponse != 'Y' && reponseReq.reponse != 'N') 
								{
								    if(msgrcv(msgid,&reponseReq,sizeof(ReponseRequete),getpid(),0) == -1)
								    {
									    printf("Erreur de lecture de la reponse de l'echangeur %d \n", voiture.entree.numEchangeur);
									    exit(1);
								    }
								}
								printf("Reponse de l'echangeur: %c\n",reponseReq.reponse);
								reponseReq.reponse='X';
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
					
					printf("-**voiture num %d est arrivee a destination**- \n",voiture.num);
					P(0);
					(*missionsAccomplies)++;
					V(0);
					printf("\n\n");
					
					
					free(voiture.itineraire);
					exit(1);
				}
				else if(pidd==-1)
				{
					printf("Erreur de creation de l'echangeur n: %d",0);
					exit(0);
				}
				
				//wait(NULL);
				sleep(1);
			}
			
				//Varariable utile afin de s'assurer que tout les v�hicules ont �t�s positionn�es avant le d�but de l'animation
			(*etatVehiculesInitialises) = 1;
			
			
			
			
				//On boucle tant que tous les v�hicules ne sont pas arriv�s � destination
			do
			{
					//On r�ceptionne la requete du v�hicule
				printf("##: %d\n",*etatVehiculesInitialises);
				if(msgrcv(msgid,&requeteV,sizeof(RequeteVoiture),1,0) == -1)
				{
					printf("Erreur de lecture de la requete de la voiture %s \n", requeteV.typeVoiture);
					exit(1);
				}
				
					//Construction et envoi de la r�ponse au v�hicule (**Temporaire**)
				reponseReq.type = requeteV.pidEmetteur;
				reponseReq.reponse = 'Y';
				if(msgsnd(msgid,&reponseReq,sizeof(ReponseRequete),0) == -1)
				{
					printf("Erreur d'envoi de la reponse � la voiture %s \n", requeteV.typeVoiture);
					exit(1);
				}
				fprintf(stdout,"missionsAccomplies: %d \n",*missionsAccomplies);
				
			}while (*missionsAccomplies+1 != atoi(argv[1]));
			
			

		/*
			while (num > 0) 
			{
				wait(NULL);
				num--;
			}
		
		*/
			
		}
		else if(pid==-1)
		{
			printf("erreur creation Echangeur n: %d",0);
			exit(0);
		}

	//}

		
		
		
		
		
		
	wait(NULL);
	fprintf(stdout,"%d - %d\n",trafficCarefour[0].NS,trafficCarefour[0].EO);
	fprintf(stdout,"missionsAccomplies: %d \n",*missionsAccomplies);
		
		
	shmdt(missionsAccomplies);
	shmdt(etatVehiculesInitialises);
	shmdt(trafficCarefour);
		
	exit(0);
}
