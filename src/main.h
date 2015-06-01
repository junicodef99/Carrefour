#ifndef __MAIN__ 
#define __MAIN__

void erreurFin(const char* msg);

void forkn(int nbFils, char *voies[], void (*fonction)());

void traitantSIGINT(int s);

void premiere_ligne(int num);

#endif
