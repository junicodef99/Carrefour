#ifndef __PROJECT__ 
#define __PROJECT__

#define MUTEX 1

#define MAXPAUSE 2

#define MESSARRIVE 1
#define MESSDEMANDE 2
#define MESSTRAVERSE 3
#define MESSATRAVERSE 4
#define MESSSORT 5

#define PASTRAVERSE 0
#define TRAVERSE 1
#define ATRAVERSE 0

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
	int voie;
	int traverse;
} Requete;

typedef struct {
	long  type;
	Voiture v;
	int autorisation;
} Reponse;

extern int tailleReq;
extern int tailleRep;

extern int croisements[25];

extern int msgid;
extern int sem_id;

#endif
