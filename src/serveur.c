#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "serveur.h"

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
