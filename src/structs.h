/**
  * Fichier qui contient les diff�rentes structures utilis�es dans le projet
**/

	//La structure position
typedef struct Position
{
	int numEchangeur;
	char orientation;
}Position;

	//**La structure voiture
typedef struct Voiture 
{
	int num;
	char type[20];
	int vitesse;
	int positionSurEchangeur;
	int tailleItineraire;
	Position entree;
	Position sortie;
	Position *itineraire;
}Voiture;

	//Structure de stock du d�gr� de traffic par orientation, par �changeur
typedef struct Traffic
{
	int N, S, E, O;
}Traffic;

	//Structure de contr�le de l'�tat des feux par echangeur
typedef struct Feu
{
	int NS, EO;
}Feu;

	//Structure pour la requette du v�hicule
typedef struct {
    long type;			
    int pidEmetteur;		//contiendra le pid de la voiture
    Position position;
    char typeVoiture[20];
}RequeteVoiture;

	//Structure pour la requette de l'�changeur
typedef struct {
    long type;			
    int pidEmetteur;		//contiendra le pid de l'�changeur
    Position positionVoiture;
}RequeteEchangeur;

	//Structure qui contiendra la r�ponse
typedef struct {
    long type;				//contiendra le pid du r�cepteur
    char reponse;
}ReponseRequete;



