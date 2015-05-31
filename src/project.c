#include <stdlib.h>
#include <stdio.h>
#include "fonctions.h"


int shmid, shmid2, shmid3;

int main(int argc, char * argv[])
{
	
	if(sem_id=semget(CLESEM1, 1, IPC_CREAT | 0666) == -1)
	{
		printf("Impossible de creer la semaphore");
		exit(1);
	}
	semctl(sem_id, 0, SETVAL, 1);
	
	shmid = shmget(CLEMEM1, sizeof(Traffic), IPC_CREAT | 0666);
	shmid2 = shmget(CLEMEM2, sizeof(int), IPC_CREAT | 0666);
	shmid3 = shmget(CLEMEM3, sizeof(int), IPC_CREAT | 0666);
	
	int msgid, msgid2;			
	key_t cle;		
	int longMSG ;	    
	RequeteVoiture requeteV;
	RequeteEchangeur requeteE;
	ReponseRequete reponseReq;
	ReponseRequete reponseReq2;
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
	if ((msgid2 = msgget(CLEMEMFILE2, 0750 | IPC_CREAT)) == -1) 
	{
		perror("Erreur de creation de la file de message\n");
		exit(1);
	}
	
	
	int pid, pidd, i, j, k, num = atoi(argv[1]);
	int *missionsAccomplies, *etatVehiculesInitialises;
	Feu statutFeu[4];
	Traffic *trafficCarefour;	
	
	initStatutFeux(statutFeu);
	
	trafficCarefour = (Traffic*)shmat(shmid, NULL, 0);
	trafficCarefour = malloc(4*sizeof(Traffic));
	initTrafficCarefour(trafficCarefour);
	
	missionsAccomplies = (int*)shmat(shmid2, NULL, 0);
	*missionsAccomplies = 0;
	
	etatVehiculesInitialises = (int*)shmat(shmid3, NULL, 0);
	*etatVehiculesInitialises = 0;
	
	printf("##: %d\n",*etatVehiculesInitialises);
	if (atoi(argv[1]) < 1 || atoi(argv[1]) > 30)
	{
		fprintf(stderr,"**Vous de devez definir en parametre un nombre de vehicules superieur a 0 et inferieur a 31**\n");
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
								//On s'assure d'abord que tous les véhicules ont étés initialisés avant de lancer l'annimation.	
							if(*etatVehiculesInitialises == 1)
							{
								voiture.positionSurEchangeur+=voiture.vitesse;
								usleep(500000);
							}
						}
						switch(voiture.entree.numEchangeur)
						{
							case 1:
								P(0);
								if(voiture.entree.orientation=='N') 
									trafficCarefour[voiture.entree.numEchangeur-1].N++;
								else if(voiture.entree.orientation=='S')
									trafficCarefour[voiture.entree.numEchangeur-1].S++;
								else if(voiture.entree.orientation=='E') 
									trafficCarefour[voiture.entree.numEchangeur-1].E++;
								else if(voiture.entree.orientation=='O')
									trafficCarefour[voiture.entree.numEchangeur-1].O++;
								V(0);
								
									//Construction et envoi de requete à l'Echangeur
								constructionRequeteVoiture(&requeteV, voiture.entree, voiture.type);
								if(msgsnd(msgid,&requeteV,sizeof(RequeteVoiture),0) == -1)
								{
									printf("Erreur d'envoi de la requete a l'echangeur %d \n", voiture.entree.numEchangeur);
									exit(1);
								}
									//On boucle tant qu'on ne reçoit pas de réponse de l'échangeur
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
					
					P(0);						
					(*missionsAccomplies)++;
					printf("la voiture %d est arrive a destination.",voiture.num);
					printf("\n\n");					
					free(voiture.itineraire);
					V(0);
					
					exit(1);
					
					
				}
				else if(pidd==-1)
				{
					printf("Erreur de creation de l'echangeur n: %d",0);
					exit(1);
				}
				
				//wait(NULL);
				sleep(1);
			}
			
				//Varariable utile afin de s'assurer que tout les véhicules ont étés positionnées avant le début de l'animation
			(*etatVehiculesInitialises) = 1;
			
			
			
			
				//On boucle tant que tous les véhicules ne sont pas arrivés à destination
			do
			{
					//On réceptionne la requete du véhicule
				if(msgrcv(msgid,&requeteV,sizeof(RequeteVoiture),1,0) == -1)
				{
					printf("Erreur de lecture de la requete de la voiture %s \n", requeteV.typeVoiture);
					exit(1);
				}	
				
							//Construction et envoie de la requete au serveur
						constructionRequeteEchangeur(&requeteE, requeteV.position);
						if(msgsnd(msgid2,&requeteE,sizeof(RequeteEchangeur),0) == -1)
						{
							printf("Erreur d'envoi de la requete au serveur par %d \n", getpid());
							exit(1);
						}
						
							//On boucle tant qu'on ne reçoit pas de réponse du serveur controleur
						while (reponseReq2.reponse != 'Y' && reponseReq2.reponse != 'N') 
						{
							if(msgrcv(msgid2,&reponseReq2,sizeof(ReponseRequete),getpid(),0) == -1)
							{
								printf("Erreur de lecture de la reponse du serveur controleur \n");
								exit(1);
							}
						}
						printf("Reponse du serveur controleur: %c\n",reponseReq2.reponse);
						
				
					//Construction et envoi de la réponse au véhicule (**Temporaire**)
				reponseReq.type = requeteV.pidEmetteur;
				if(reponseReq2.reponse == 'Y')		
					reponseReq.reponse = 'Y';
				else
					reponseReq.reponse = 'N';
					//On envoi une réponse au véhicule
				if(msgsnd(msgid,&reponseReq,sizeof(ReponseRequete),0) == -1)
				{
					printf("Erreur d'envoi de la reponse à la voiture %s \n", requeteV.typeVoiture);
					exit(1);
				}
						reponseReq2.reponse='X';
				
				fprintf(stdout,"missionsAccomplies: %d \n",(*missionsAccomplies)+1);
				usleep(10000);
			}while (*missionsAccomplies != atoi(argv[1]));
			

		
			//while (*missionsAccomplies != atoi(argv[1])) {printf("%d\n",0);}
			
			while ((pid=waitpid(-1,NULL,0)) != -1){}
		
			exit(1);
		}
		else if(pid==-1)
		{
			printf("erreur creation Echangeur n: %d",0);
			exit(1);
		}

	//}
		
		
	
		
		
		//On boucle tant que tous les véhicules ne sont pas arrivés à destination
	do
	{
			//On réceptionne la requete de l'échangeur
		if(msgrcv(msgid2,&requeteE,sizeof(RequeteEchangeur),1,0) == -1)
		{
			printf("Erreur de lecture de la requete de l'echangeur \n");
			exit(1);
		}		
	
			//Construction et envoi de la réponse à l'échangeur (**Temporaire**)
		reponseReq2.reponse=demandeAutorisation(&(requeteE.positionVoiture),trafficCarefour,statutFeu);
		reponseReq2.type = requeteE.pidEmetteur;
			//On envoi une réponse à l'échangeur
		if(msgsnd(msgid2,&reponseReq2,sizeof(ReponseRequete),0) == -1)
		{
			printf("Erreur d'envoi de la reponse a l'echangeur %d\n", requeteE.positionVoiture.numEchangeur);
			exit(1);
		}
				
		fprintf(stdout,"le serveur controleur a reagit. \n");
		usleep(5000);
	}while (*missionsAccomplies != atoi(argv[1]));		
		
	while ((pid=waitpid(-1,NULL,0)) != -1){}
		
		
	fprintf(stdout,"N:%d - S:%d - E:%d - O:%d\n",trafficCarefour[0].N,trafficCarefour[0].S,trafficCarefour[0].E,trafficCarefour[0].O);
	fprintf(stdout,"missionsAccomplies: %d \n",*missionsAccomplies);
		
	/*
	if (semctl(sem_id,0,IPC_RMID,NULL)==-1)
	{
		perror("impossible de detruire le semaphore") ;
		exit(1) ;
	}
	*/	
	
	shmdt(missionsAccomplies);
	shmdt(etatVehiculesInitialises);
	shmdt(trafficCarefour);
		
	exit(0);
}
