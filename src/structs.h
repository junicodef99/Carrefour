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

	//La structure de stock du dégré de traffic par direction, par échangeur
typedef struct Traffic
{
	int NS;
	int EO;
}Traffic;

	//Structure pour la requette du véhicule
typedef struct {
    long type;			//contiendra le pid de la voiture
    int pidEmetteur;
    Position position;
    char typeVoiture[20];
}RequeteVoiture;

	//Structure pour la requette de l'échangeur
typedef struct {
    long type;			//contiendra le pid de l'échangeur
    int pidEmetteur;
    Position positionVoiture;
}RequeteEchangeur;

	//Structure qui contiendra la réponse
typedef struct {
    long type;				//contiendra le pid du récepteur
    char reponse;
}ReponseRequete;





