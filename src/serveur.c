#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "serveur.h"

/*---------------- CODE SERVEUR -----------------------*/

void constructionReponse(trequete *req,treponse *rep)
{

	rep->type = req->pidEmetteur;
	rep->autorisation = 1;
	
//	if (req->type == 1) printf("Voiture %d proc %d arrive voie %d\n", req->v.numero, req->pidEmetteur, req->v.voie->numero);
//	else if (req->type == 2) printf("Voiture %d proc %d demande a passer intersection %d voie %d\n", req->v.numero, req->pidEmetteur, req->croisement, req->v.voie->numero);
	
	
/*			val = semctl(sem_id, croisement-1, GETVAL, NULL);
			if (val == 0) {
				P(21);
				snprintf(buffer, sizeof(buffer), "Priorite a voie %d\n", croisement);
				message(voiture.num, buffer);
				V(21);
				P(s);
				P(21);
				snprintf(buffer, sizeof(buffer), "Repart voie %d\n", croisement);
				message(voiture.num, buffer);
				V(21);
			} else {
				P(s);
			}
			P(21);
			snprintf(buffer, sizeof(buffer), "Traverse voie %d\n", croisement);
			message(voiture.num, buffer);
			V(21);
			sleep(rand()%MAXPAUSE);
			P(21);
			snprintf(buffer, sizeof(buffer), "A traverse voie %d\n", croisement);
			message(voiture.num, buffer);
			V(21);
			V(s);
*/

}

void affichageReponse(trequete *req,treponse *rep)
{

}

void maj_carrefour(trequete *req)
{

}
