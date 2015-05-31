#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "voiture.h"

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
							
void initRand()
{
	srand(getpid());
}

void voiture(int numero, int voie)
{
	Voiture v;
	trequete req;
	treponse rep;
	int croisement, i;
	
	if (voie == -1) {
		initRand();
		v.numero = numero;
		int voie_random = rand()%12+1;
		v.voie = &voies[voie_random-1];
	} else {
		v.numero = numero;
		v.voie = &voies[voie-1];
	}

/*
	tailleRep = sizeof(int);
	break;
	msgrcv(msgid,&rep,tailleRep,getpid(),0);
	affichageReponse(&req,&rep);
	break;
	}
*/

	P(MUTEX);
	constructionRequete(&req, &v, 0, MESSARRIVE);
	affichageRequete(&req);
	msgsnd(msgid,&req,tailleReq,0);
	V(MUTEX);

	for (i=0 ; i < 6 ; i++) {
			croisement = v.voie->croisements[i];
			
			if (croisement == -1) break;
			
			P(MUTEX);
			maj_position(&v, croisement, PASTRAVERSE);
			constructionRequete(&req, &v, croisement, MESSDEMANDE);
			affichageRequete(&req);
			V(MUTEX);
			
			do {
				msgsnd(msgid,&req,tailleReq,0);
				msgrcv(msgid,&rep,tailleRep,getpid(),0);
			} while (rep.autorisation == 0);
			
			P(MUTEX);
			maj_position(&v, croisement, TRAVERSE);
			constructionRequete(&req, &v, croisement, MESSTRAVERSE);
			affichageRequete(&req);
			msgsnd(msgid,&req,tailleReq,0);
			V(MUTEX);
			
			sleep(rand()%MAXPAUSE);
			
			P(MUTEX);
			maj_position(&v, croisement, ATRAVERSE);
			constructionRequete(&req, &v, croisement, MESSATRAVERSE);
			affichageRequete(&req);
			msgsnd(msgid,&req,tailleReq,0);
			V(MUTEX);
	}

	P(MUTEX);	
	constructionRequete(&req, &v, 0, MESSSORT);
	affichageRequete(&req);
	msgsnd(msgid,&req,tailleReq,0);
	V(MUTEX);
}

void maj_position(Voiture *v, int position, int traverse)
{
	v->position = position;
	v->position_traversee = traverse;	
}
