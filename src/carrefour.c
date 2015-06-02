#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "carrefour.h"

#define TAILLE 5

int croisements[25] = {0,0,-1,0,0,0,0,0,0,0,-1,0,-1,0,-1,0,0,0,0,0,0,0,-1,0,0};

void carrefour()
{
	Requete req;
	Reponse rep;

	initRand();

	while (1) {
		msgrcv(msgid,&req,3*tailleReq,0,0);

		if ((req.type == MESSDEMANDE) || (req.type == MESSTRAVERSE) || (req.type == MESSATRAVERSE))
		maj_carrefour(&req);

		if (req.type == MESSDEMANDE) {
			constructionReponse(&req,&rep);
			msgsnd(msgid,&rep,tailleRep,0);
		}
	}
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
