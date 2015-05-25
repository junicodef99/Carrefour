#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/sem.h> 
#include <errno.h>
#include <string.h>
#include <time.h>
#include "structs.h"

/**
  * Fichier qui contient les fonctions de gestion du projet
**/

int sem_id;
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

void initTrafficCarefour(Traffic* trafficC)
{
	int i;
	for(i=0; i<4; i++)
	{
		trafficC[i].NS=0;
		trafficC[i].EO=0;
	}
}

void initProprietesVehicule(Voiture* V)
{
	char *listeTypesV[10] = {"ordinaire","ordinaire","police","ordinaire","ordinaire","ambulance","ordinaire","ordinaire","pompier","ordinaire"};
	int tmpOrientation, degresVitesses[3] = {1,2,5};
	char OrientationEchangeur[4] = {'N','E','S','O'};
	
	V->num = getpid();
	srand(time(NULL));
	strcpy(V->type,listeTypesV[rand()%10]);
	//sleep(1);
	srand(time(NULL));
	V->vitesse = degresVitesses[rand()%3];
	V->positionSurEchangeur=-10;
	//sleep(1);
	srand(time(NULL));
	V->entree.numEchangeur = 1; //+rand()%4;
	V->entree.orientation = OrientationEchangeur[rand()%4];
	//sleep(1);
	V->sortie.numEchangeur = 1; 
	if(V->entree.numEchangeur != V->sortie.numEchangeur)
		V->sortie.orientation = OrientationEchangeur[rand()%4];
	else
	{	
		do
		{
			V->sortie.orientation = OrientationEchangeur[rand()%4];
	
		}while(V->sortie.orientation == V->entree.orientation);
	}

		//Itineraire -- provisoire (pour un echangeur)
	
	V->tailleItineraire = 1;
	if(V->entree.numEchangeur==V->sortie.numEchangeur)
	{
		V->itineraire = malloc(V->tailleItineraire*sizeof(Position));
		V->itineraire[0].numEchangeur = V->entree.numEchangeur;
		V->itineraire[0].orientation = V->sortie.orientation;
	}
	
	
}
/*
void initVoiture(int nbre)
{
	int i, j, pid; 
	
	
}
*/


