#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define TAILLE 10 // Permet de définir la taille du serpent
#define VITESSE 20000 // Permet de définir la vitesse du serpent
#define STOP 'a' // Touche à presser pour arrêter le programme
#define CORPS 'X' // Caractère composant le corps du serpent
#define TETE 'O' // Caractère représentant la tête du serpent
#define HAUT 'z'
#define BAS 's'
#define GAUCHE 'q'
#define DROITE 'd'

void gotoXY(int x, int y);

void afficher(int x, int y, char c);

void effacer(int x,int y);

void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]);

void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction);

void testInput(char direction);

int kbhit();

void disableEcho();

void enableEcho();

int main () {
    int x, y, i; //Variables coordonnées et variable incrémentation
	int lesX[TAILLE], lesY[TAILLE]; // Variables tableaux pour placer serpent
	char input='x'; // Variable pour mettre contenir caractère qui met fin au programme ou indique la direction

	x=20;
	y=20;

	for (i=0; i<TAILLE; i++)
	{
		lesX[i]=x-i;
		lesY[i]=y;
	}
	// Permet de placer le tableau contenant le serpent dans le terminal

	system("clear"); // Efface le terminal
	dessinerSerpent(lesX, lesY);
	disableEcho();
	input='d';

	while (input != STOP) {
		usleep(VITESSE);
		progresser(lesX, lesY, input);
		if (kbhit()==1) 
		{
			input=getchar();
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

void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]) {
    for (int i=0; i<TAILLE; i++)
    {
		if (lesX[i]>0) 
		{
			afficher(lesX[i],lesY[i],CORPS);
		}
    }
	afficher(lesX[0],lesY[0],TETE);
}
// Permet affichage du serpent dans tableau prévu à cet effet en appellant procédure afficher pour dessiner le serpent.

void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction) {
	effacer(lesX[TAILLE-1],lesY[TAILLE-1]);
	for (int i=TAILLE-1; i>0; i--)
	{
		lesX[i]=lesX[i-1];
		lesY[i]=lesY[i-1];
	}
	switch (direction) {
		case HAUT :
			lesY[0]=lesY[0]-1;
			break;
		case BAS :
			lesY[0]=lesY[0]+1;
			break;
		case GAUCHE :
			lesX[0]=lesX[0]-1;
			break;
		case DROITE :
			lesX[0]=lesX[0]+1;
			break;
	}
	dessinerSerpent(lesX, lesY);
}
// Permet d'effacer le serpent puis le réaffiche une case plus loin sur la droite.

void testInput(char direction) {
	
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