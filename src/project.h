#ifndef __PROJECT__ 
#define __PROJECT__

#define MUTEX 21

#define MAXPAUSE 2

#define MESSARRIVE 1
#define MESSDEMANDE 2
#define MESSTRAVERSE 3
#define MESSATRAVERSE 4
#define MESSSORT 5

#define PASTRAVERSE -1
#define TRAVERSE 0
#define ATRAVERSE 1

typedef struct Voie {
	int numero;
	int sem_num[6];
	int croisements[6];
} Voie;

typedef struct Voiture {
	int numero;
	Voie *voie;
	int position;
	int position_traversee;
} Voiture;

typedef struct {
	long  type;
	pid_t pidEmetteur;
	Voiture v;
	int croisement;
} trequete;

typedef struct {
	long  type;
	Voiture v;
	int autorisation;
} treponse;

//extern int tailleReq = sizeof(trequete) - sizeof(long);
//extern int tailleRep = sizeof(treponse);

extern int tailleReq;
extern int tailleRep;

extern int msgid;
extern int sem_id;

#endif
