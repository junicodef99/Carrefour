#ifndef __VOITURE__ 
#define __VOITURE__

#include "project.h"

void affiche_voiture(Voiture *v);

void initRand();

void voiture(int numero, int voie);

void maj_position(Voiture *v, int position, int traverse);

#endif