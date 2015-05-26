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

	//La structure de stock du d�gr� de traffic par direction, par �changeur
typedef struct Traffic
{
	int NS;
	int EO;
}Traffic;

	//Structure pour la requette du v�hicule
typedef struct {
    long type;			//contiendra le pid de la voiture
    int pidEmetteur;
    Position position;
    char typeVoiture[20];
}RequeteVoiture;

	//Structure pour la requette de l'�changeur
typedef struct {
    long type;			//contiendra le pid de l'�changeur
    int pidEmetteur;
    Position positionVoiture;
}RequeteEchangeur;

	//Structure qui contiendra la r�ponse
typedef struct {
    long type;				//contiendra le pid du r�cepteur
    char reponse;
}ReponseRequete;





