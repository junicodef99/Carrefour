#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "carrefour.h"

void serveur()
{
	trequete req;
	treponse rep;

	initRand();

	while (1) {
		msgrcv(msgid,&req,tailleReq,0,0);

		maj_carrefour(&req);
		
		if (req.type == MESSDEMANDE) {
			constructionReponse(&req,&rep);
			msgsnd(msgid,&rep,tailleRep,0);
		}
	}
}
