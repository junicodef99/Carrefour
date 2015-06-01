#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "voiture.h"

Voie voies[12] = {	{1,{15,16,17,13,8,3},{11,7,10,4,7,8}},
					{2,{20,21,23,24,-1},{11,7,4,5,-1}},
					{3,{-1},{-1}},
					{4,{23,18,13,7,6,5},{2,10,1,7,10,11}},
					{5,{24,19,9,4,-1},{2,10,7,8,-1}},
					{6,{-1},{-1}},
					{7,{9,8,7,11,16,21},{5,1,4,10,1,2}},
					{8,{4,3,1,0,-1},{5,1,10,11,-1}},
					{9,{-1},{-1}},
					{10,{1,6,11,17,18,19},{8,4,7,1,4,5}},
					{11,{0,5,15,20,-1},{8,4,1,2,-1}},
					{12,{-1},{-1}}	};

void initRand()
{
	srand(getpid());
}

void affiche_voiture(Voiture *v)
{
	printf("Numero : %d\n", v->numero);
	printf("Voie : %d\n", v->voie->numero);

	int i=0;
	printf("Croisements : {");
	while (v->voie->sem_num[i] != -1) {
		printf("%d,", v->voie->sem_num[i]);
		i++;
	}
	printf("}\n");
}

void voiture(int numero, int voie)
{
	Voiture v;
	Requete req;
	Reponse rep;
	int croisement_numero, croisement_voie, i;

	v.numero = numero;

	if (voie == -1) {
		initRand();
		int voie_random = rand()%12+1;
		v.voie = &voies[voie_random-1];
	} else
		v.voie = &voies[voie-1];

	P(MUTEX);
	constructionRequete(&req, &v, 0, v.voie->numero, 0, MESSARRIVE);
	affichageRequete(&req);
	msgsnd(msgid,&req,tailleReq,0);
	V(MUTEX);

	for (i=0 ; i < 6 ; i++) {
		croisement_numero = v.voie->sem_num[i];
		croisement_voie = v.voie->croisements[i];

		if (croisement_numero == -1) break;

		P(MUTEX);
		maj_position(&v, croisement_numero, PASTRAVERSE);
		constructionRequete(&req, &v, croisement_numero, croisement_voie, PASTRAVERSE, MESSDEMANDE);
		affichageRequete(&req);
		V(MUTEX);

		do {
			msgsnd(msgid,&req,tailleReq,0);
			msgrcv(msgid,&rep,tailleRep,getpid(),0);
		} while (rep.autorisation == 0);

		P(MUTEX);
		maj_position(&v, croisement_numero, TRAVERSE);
		constructionRequete(&req, &v, croisement_numero, croisement_voie, TRAVERSE, MESSTRAVERSE);
		affichageRequete(&req);
		msgsnd(msgid,&req,tailleReq,0);
		V(MUTEX);

		sleep(rand()%MAXPAUSE);

		P(MUTEX);
		maj_position(&v, croisement_numero, ATRAVERSE);
		constructionRequete(&req, &v, croisement_numero, croisement_voie, ATRAVERSE, MESSATRAVERSE);
		affichageRequete(&req);
		msgsnd(msgid,&req,tailleReq,0);
		V(MUTEX);
	}

	P(MUTEX);
	constructionRequete(&req, &v, 0, v.voie->numero, 0, MESSSORT);
	affichageRequete(&req);
	msgsnd(msgid,&req,tailleReq,0);
	V(MUTEX);
}

void maj_position(Voiture *v, int position, int traverse)
{
	v->position = position;
	v->position_traversee = traverse;
}