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
    int pidVoiture;
    Position position;
    char type[20];
}requeteVoiture;

	//Structure pour la requette de l'échangeur
typedef struct {
    int numEchangeur;
    Position positionVoiture;
}requeteEchangeur;





