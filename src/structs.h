/**
  * Fichier qui contient les différentes structures utilisées dans le projet
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

	//Structure de stock du dégré de traffic par orientation, par échangeur
typedef struct Traffic
{
	int N, S, E, O;
}Traffic;

	//Structure de contrôle de l'état des feux par echangeur
typedef struct Feu
{
	int NS, EO;
}Feu;

	//Structure pour la requette du véhicule
typedef struct {
    long type;			
    int pidEmetteur;		//contiendra le pid de la voiture
    Position position;
    char typeVoiture[20];
}RequeteVoiture;

	//Structure pour la requette de l'échangeur
typedef struct {
    long type;			
    int pidEmetteur;		//contiendra le pid de l'échangeur
    Position positionVoiture;
}RequeteEchangeur;

	//Structure qui contiendra la réponse
typedef struct {
    long type;				//contiendra le pid du récepteur
    char reponse;
}ReponseRequete;



