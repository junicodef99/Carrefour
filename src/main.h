#ifndef __MAIN__ 
#define __MAIN__

void erreurFin(const char* msg);

void forkn(int nbFils, int voie, void (*pauBoulot)());

void traitantSIGINT(int s);

#endif
