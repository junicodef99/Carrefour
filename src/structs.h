/**
  * Fichier qui contient les différentes structures.
**/

	//La structure position
typedef struct Position
{
	int num_echangeur;
	char orientation_dentree;
	int pos_sur_echangeur;
}Position;

	//La structure destination
typedef struct Destination
{
	int num_echangeur;
	char orientation_de_sortie;
}Destination;

	//**La structure voiture
typedef struct Voiture 
{
	int num;
	char type[20];
	int vitesse;
	Position position;
	Destination destination;
}Voiture;

