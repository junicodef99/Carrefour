#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "carrefour.h"

void serveur()
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