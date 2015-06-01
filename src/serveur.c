#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "serveur.h"

#define TAILLE 5

int croisements[25] = {0,0,-1,0,0,0,0,0,0,0,-1,0,-1,0,-1,0,0,0,0,0,0,0,-1,0,0};

/*---------------- CODE SERVEUR -----------------------*/

void constructionReponse(Requete *req,Reponse *rep)
{
	rep->type = req->pidEmetteur;

	if (croisements[req->croisement] == 0)
		rep->autorisation = 1;
	else
		rep->autorisation = 1;
}

void affichageReponse(Requete *req,Reponse *rep)
{

}

void maj_carrefour(Requete *req)
{
	int i = req->croisement;
	int etat = req->traverse;
	croisements[i] = etat;
//	affiche_carrefour();
}

void affiche_carrefour()
{
	P(MUTEX);
	
	int i, j;

	printf("\n--------------------------------------------\n");
	for (i=0;i<TAILLE;i++) {
		for (j=0;j<TAILLE;j++) {
			printf("%d\t", croisements[i*TAILLE+j]);
		}
		printf("\n");
	}
	printf("--------------------------------------------\n");
	
	V(MUTEX);
}