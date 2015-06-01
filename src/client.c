#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "interface.h"
#include "client.h"

/*---------------- CODE CLIENT -----------------------*/

void constructionRequete(Requete *req, Voiture *v, int croisement, int voie, int traverse, int type)
{
	req->pidEmetteur = getpid();
	req->v = *v;
	req->croisement = croisement;
	req->voie = voie;
	req->traverse = traverse;
	req->type = type;
}

void affichageRequete(Requete *req)
{
	if (req->type == MESSARRIVE)
		snprintf(buffer, sizeof(buffer), "Arrive voie %d\n", req->voie);
	else if (req->type == MESSDEMANDE)
		snprintf(buffer, sizeof(buffer), "Dem. voie %d\n", req->voie);
	else if (req->type == MESSTRAVERSE)
		snprintf(buffer, sizeof(buffer), "Traverse voie %d\n", req->voie);
	else if (req->type == MESSATRAVERSE)
		snprintf(buffer, sizeof(buffer), "A traverse voie %d\n", req->voie);
	else if (req->type == MESSSORT)
		snprintf(buffer, sizeof(buffer), "Sort voie %d\n", req->voie);

	message(req->v.numero, buffer);
}