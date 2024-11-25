#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

#define TAILLE 10 // Permet de définir la taille du serpent
#define MAX_TAILLE 30 // Permet de définir la taille maximale du serpent
#define VITESSE 150000 // Permet de définir la vitesse du serpent
#define Y_DEPART 20 // Coordonnée y de départ du snake
#define X_DEPART 40 // Coordonnée x de départ du snake
#define HAUTEUR 40 // Hauteur du plateau
#define LARGEUR 80 // Largeur du plateau
#define TAILLE_PAVE 5 // Dimensions des pavés 
#define MUR '#' // Caractère bordure ou pavé
#define POMME '6' // Caractère de la pomme
#define NB_POMME 1 // Permet de faire apparaitre plusieurs pommes au départ pour tester l'apparition correcte
#define VICTOIRE 10 // Nombre de pommes à manger pour gagner
#define NB_PAVES 4 // Nombre de pavés sur le plateau
#define STOP 'a' // Touche à presser pour arrêter le programme
#define CORPS 'X' // Caractère composant le corps du serpent
#define TETE 'O' // Caractère représentant la tête du serpent
#define HAUT 'z' // Touche à presser pour que le serpent monte
#define BAS 's' // Touche à presser pour que le serpent descende
#define GAUCHE 'q' // Touche à presser pour que le serpent aille à gauche
#define DROITE 'd' // Touche à presser pour que le serpent aille à droite

int tailleSerpent = TAILLE; // Variable globale pour la taille du serpent qui augmente

void gotoXY(int x, int y);

void afficher(int x, int y, char c);

void effacer(int x,int y);

void dessinerSerpent(int lesX[MAX_TAILLE], int lesY[MAX_TAILLE]);

void progresser(int lesX[MAX_TAILLE], int lesY[MAX_TAILLE], char direction, char plateau[HAUTEUR][LARGEUR], bool *check, bool *pommeMangee);

void initPlateau(char plateau[HAUTEUR][LARGEUR]);

void dessinerPlateau(char plateau[HAUTEUR][LARGEUR]);

void ajouterPomme(char plateau[HAUTEUR][LARGEUR]);

int kbhit();

void disableEcho();

void enableEcho();

int main () {
    int x, y, i; //Variables coordonnées et variable incrémentation
    int pommesMangees = 0; // Variable pour compter le nombre de pommes mangées
    int vitesseActuelle = VITESSE; // Variable pour la vitesse actuelle du serpent
	int lesX[MAX_TAILLE], lesY[MAX_TAILLE]; // Variables tableaux pour placer serpent
	char plateau[HAUTEUR][LARGEUR]; // Variable tableau pour le plateau de jeu
	char input, direction; // Variable pour mettre contenir caractère qui met fin au programme ou indique la direction
	bool check, pommeMangee; // Variable booléen pour tester la collision
    int a;

	x=X_DEPART;
	y=Y_DEPART;
	input=DROITE; // Le serpent va à droite par défaut
	check=false; // Pas de collision par défaut
    pommeMangee=false; // Pas de pomme mangée par défaut

	for (i=0; i<TAILLE; i++)
	{
		lesX[i]=x-i;
		lesY[i]=y;
	}
	// Permet de placer le tableau contenant le serpent dans le terminal

	initPlateau(plateau); 
	dessinerPlateau(plateau);
    dessinerSerpent(lesX, lesY);
    for(a = 0; a < NB_POMME; a++) { // pour tester la bonne apparition des pommes
        ajouterPomme(plateau);
    }

	disableEcho();
	direction='d'; // Le sepent va vers la droite par défaut

	while (input != STOP && check != true) { // Fais la boucle tant que pas de collision ou que 'a' n'est pas pressé
		usleep(vitesseActuelle);
		progresser(lesX, lesY, direction, plateau, &check, &pommeMangee); // Mise à jour de check à chaque itération de la boucle 
        if (pommeMangee == true) { // Si la pomme a été mangée
            pommeMangee = false; // Réinitialise la variable pour la prochaine pomme
            ajouterPomme(plateau); // Ajoute une nouvelle pomme
            pommesMangees = pommesMangees +1; // Incrémente le nombre de pommes mangées
            vitesseActuelle = vitesseActuelle - 10000; // Augmente la vitesse du serpent
        }
		if (kbhit()==1) 
		{
			input=getchar();
		}
		if (input==STOP || input==HAUT || input==BAS || input==GAUCHE || input==DROITE) { // N'autorise que les bonnes touches
			if (input!=direction) {
				if (direction==DROITE && input!=GAUCHE) { // Empêche le sepent de retourner sur ses pas
					direction=input;
				}
				else if (direction==GAUCHE && input!=DROITE) { // Empêche le sepent de retourner sur ses pas
					direction=input;
				}
				else if (direction==HAUT && input!=BAS) { // Empêche le sepent de retourner sur ses pas
					direction=input;
				}
				else if (direction==BAS && input!=HAUT) { // Empêche le sepent de retourner sur ses pas
					direction=input;
				}
			}
		}
        if(pommesMangees >= 10){ // Si le serpent a mangé 10 pommes
            system("clear");
            printf("Partie gagnée ! Vous avez mangé 10 pommes.\n"); // Affiche un message de victoire
            exit(0); // Arrête le programme
        }
	}
	enableEcho();
	return EXIT_SUCCESS;
}

void gotoXY(int x, int y) { // Pointe une coordonnée
    printf("\033[%d;%df", y, x);
}

void afficher(int x, int y, char c) { // Permet d'afficher un caractère sur une coordonnée précise
    gotoXY(x,y);
    printf("%c", c);
}

void effacer(int x,int y) { // Permet d'effacer un caractère sur une coordonnée précise
	gotoXY(x, y);
	printf(" ");
}

void dessinerSerpent(int lesX[MAX_TAILLE], int lesY[MAX_TAILLE]) {
    for (int i=0; i<tailleSerpent; i++)
    {
		if (lesX[i]>0 && lesY[i]>0) // Empêche le serpent de s'afficher dans les coordonnées négatives
		{
			afficher(lesX[i],lesY[i],CORPS);
		}
    }
	if (lesX[0]>0 && lesY[0]>0) { // Empêche le serpent de s'afficher dans les coordonnées négatives
		afficher(lesX[0],lesY[0],TETE);
	}
}
// Permet affichage du serpent dans tableau prévu à cet effet en appellant procédure afficher pour dessiner le serpent.

void progresser(int lesX[MAX_TAILLE], int lesY[MAX_TAILLE], char direction, char plateau[HAUTEUR][LARGEUR], bool *check, bool *pommeMangee) {
    int nouvelleX = lesX[0]; // Nouvelle variable intermédiaire pour stocker la nouvelle position en x
    int nouvelleY = lesY[0]; // Nouvelle variable intermédiaire pour stocker la nouvelle position en y

    switch (direction) { // Détermine la nouvelle position en fonction de la direction
        case HAUT : 
            nouvelleY--;
            break;
        case BAS : 
            nouvelleY++;
            break;
        case GAUCHE : 
            nouvelleX--;
            break;
        case DROITE : 
            nouvelleX++;
            break;
    }

    // Vérifier si une pomme est mangée
    if (plateau[nouvelleY][nouvelleX] == POMME) { // Si la nouvelle position est occupée par une pomme
        *pommeMangee = true; // La pomme a été mangée
        plateau[nouvelleY][nouvelleX] = ' '; // La pomme est effacée
    }

    // Vérification des bords pour téléportation
    if (nouvelleY < 1) { // Si le serpent passe par le haut
        nouvelleY = HAUTEUR; // Téléportation en bas
    }
    else if (nouvelleY > HAUTEUR) { // Si le serpent passe par le bas
        nouvelleY = 1; // Téléportation en haut
    }
    if (nouvelleX < 1) { // Si le serpent passe par la gauche
        nouvelleX = LARGEUR; // Téléportation à droite
    }
    else if (nouvelleX > LARGEUR) { // Si le serpent passe par la droite
        nouvelleX = 1; // Téléportation à gauche
    }

    // Vérification des collisions avec bordure ou pavé
    if (plateau[nouvelleY-1][nouvelleX-1] == MUR) {
        *check = true; // Collision détectée
    }

    // Vérification des collisions avec le corps du serpent
    for (int i = 0; i < tailleSerpent; i++) {
        if (nouvelleX == lesX[i] && nouvelleY == lesY[i]) {
            *check = true; // Collision avec soi-même
        }
    }

    if (*check != true) {
        // Si une pomme a été mangée
        if (*pommeMangee == true) {
            // On ne supprime pas la dernière position, on augmente la taille du serpent
            tailleSerpent += 1;
        } else {
            // Effacer la dernière position du serpent
            effacer(lesX[tailleSerpent - 1], lesY[tailleSerpent - 1]);
        }

        // Déplacement du serpent
        for (int i = tailleSerpent - 1; i > 0; i--) {
            lesX[i] = lesX[i - 1];
            lesY[i] = lesY[i - 1];
        }
        lesX[0] = nouvelleX;
        lesY[0] = nouvelleY;
        dessinerSerpent(lesX, lesY);
    }
}
// Permet d'effacer le serpent puis le réaffiche une case plus loin sur la droite.

void initPlateau(char plateau[HAUTEUR][LARGEUR]) {

    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            plateau[i][j] = ' ';
        }
    } // Initialisation du plateau avec des espaces

    for (int i = 0; i < HAUTEUR; i++) {
        if (i!=(HAUTEUR/2)) {
            plateau[i][0] = MUR;
            plateau[i][LARGEUR - 1] = MUR;
        }
    }
    for (int j = 0; j < LARGEUR; j++) {
        if (j!=(LARGEUR/2)) {
            plateau[0][j] = MUR;
            plateau[HAUTEUR - 1][j] = MUR;
        }
    } // Ajout des bordures

    // Placement aléatoire des pavés
    srand(time(NULL));
    for (int p = 0; p < NB_PAVES; p++) {
        int x, y;
        bool valide;
        do {
            valide = true;
            // Position aléatoire pour le coin supérieur gauche du pavé
            x = rand() % (LARGEUR - TAILLE_PAVE - 2) + 1;
            y = rand() % (HAUTEUR - TAILLE_PAVE - 2) + 1;

            // Vérification que le pavé ne touche pas les bordures
            if (x == 1 || y == 1 || x + TAILLE_PAVE >= LARGEUR - 1 || y + TAILLE_PAVE >= HAUTEUR - 1) {
                valide = false;
            }

            // Vérification que le pavé n'est pas dans la zone d'apparition du serpent
            if ((x <= X_DEPART+1 && x >= X_DEPART-(TAILLE+TAILLE_PAVE)) && (y <= Y_DEPART && y >= Y_DEPART-TAILLE_PAVE)) {
                valide = false;
            }
        } while (valide==false);

        // Dessin du pavé
        for (int i = 0; i < TAILLE_PAVE; i++) {
            for (int j = 0; j < TAILLE_PAVE; j++) {
                plateau[y + i][x + j] = MUR;
            }
        }
    }
}

// Procédure pour ajouter une pomme aléatoirement sur le plateau
void ajouterPomme(char plateau[HAUTEUR][LARGEUR]) {
    int x, y;
    bool valide;
    srand(time(NULL));
    do {
        valide = false;
        x = rand() % (LARGEUR - 2) + 2; // Génère une position aléatoire
        y = rand() % (HAUTEUR - 2) + 2;

        if (plateau[y][x] == ' ') { // Vérifie que la position est vide
            valide = true; // Si la position est vide, on la considère comme valide
        }

    } while (valide==false); // Trouve une case libre

    plateau[y][x] = POMME; // Place la pomme
    afficher(x, y, POMME); // Affiche la pomme
}

void dessinerPlateau(char plateau[HAUTEUR][LARGEUR]) {
    // Effacement de l'écran
    system("clear");

    // Affichage du plateau
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            putchar(plateau[i][j]);
        }
        putchar('\n');
    }
}

int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}