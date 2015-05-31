#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "interface.h"
#include "client.h"

/*---------------- CODE CLIENT -----------------------*/

void constructionRequete(trequete *req, Voiture *v, int croisement, int type)
{
	req->pidEmetteur = getpid();
	req->v = *v;
	req->croisement = croisement;
	req->type = type;
}

void affichageRequete(trequete *req)
{
	if (req->type == MESSARRIVE)
		snprintf(buffer, sizeof(buffer), "Arrive voie %d\n", req->v.voie->numero);
	else if (req->type == MESSDEMANDE)
		snprintf(buffer, sizeof(buffer), "Dem. int. %d\n", req->croisement);
	else if (req->type == MESSTRAVERSE)
		snprintf(buffer, sizeof(buffer), "Traverse voie %d\n", req->croisement);
	else if (req->type == MESSATRAVERSE)
		snprintf(buffer, sizeof(buffer), "A traverse voie %d\n", req->croisement);
	else if (req->type == MESSSORT)
		snprintf(buffer, sizeof(buffer), "Sort voie %d\n", req->v.voie->numero);

	message(req->v.numero, buffer);
}


