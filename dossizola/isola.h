
// Tailles de la grille
#define XCASEMIN 4
#define YCASEMIN 4
#define XCASEMAX 9
#define YCASEMAX 7
#define MAXCASES 10

// Vitesse des animation
#define TREMBLE_NB 10			// Nombre d'it�rations du tremblement de la grille
#define TREMBLE_DECAL 6			// D�callage maximum du tremblement de la grille
#define TREMBLE_VITESSE 2		// Vitesse du tremblement
#define BOULE_VITESSE 1			// Vitesse de l'animation de la boule
#define CARACT_VITESSE 5		// Vitesse d'apparition des caract�re
#define TXT_DECALX -150			// Position des textes du menu sur l'axe des X
#define TXT_DECALY -140			// Position des textes du menu sur l'axe des Y

// Valeurs de jeu par d�faut
#define DFT_J1ROUGE NON
#define DFT_J1TYPE HUMAIN
#define DFT_J2TYPE HUMAIN
#define DFT_J1NIVO 2
#define DFT_J2NIVO 2
#define DFT_INTEMP NON
#define DFT_DOUBLE_DEPLACE NON
#define DFT_DOUBLE_BOULEFEU NON
#define DFT_XGRILLE 6
#define DFT_YGRILLE 6
#define DFT_FPS 64

// Textes
#define TXT_OUI			"YES"
#define TXT_NON			"NO"
#define TXT_SIMPLE		"SIMPLE"
#define TXT_DOUBLE		"DOUBLE"
#define TXT_INDISPONIBLE "NOT YET AVAILABLE"
#define TXT_PATIENTEZ	"PLEASE WAIT"
#define TXT_J1NOM		"PLAYER 1"
#define TXT_J2NOM		"PLAYER 2"
#define TXT_HUMAIN		"HUMAN"
#define TXT_ORDI		"COMPUTER"
#define TXT_JOUER		"START"
#define TXT_AIDE		"HELP"
#define TXT_QUITTER		"EXIT"
#define TXT_REFAIRE		"RESTART"
#define TXT_MENU		"MENU"
#define TXT_CONSEIL		"ADVICE"
#define TXT_QSTQUITTER	" EXIT NOW :"
#define TXT_QSTMENU		" BACK TO MENU :"
#define TXT_QSTREFAIRE	" RESTART GAME :"
#define TXT_RETOURMENU	"CLIC TO GO BACK TO MENU"
#define TXT_AIDE1		"GOAL OF THE GAME"
#define TXT_AIDE2		"YOU MUST BLOCK THE OPPONENT"
#define TXT_AIDE3		"BY DESTROYING ALL THE PADS"
#define TXT_AIDE4		"WHICH SURROUND HIM BEFORE"
#define TXT_AIDE5		"BEING BLOCKED YOURSELF"
#define TXT_AIDE6		"HOW TO PLAY"
#define TXT_AIDE7		"FOR EACH TURN OF PLAY YOU"
#define TXT_AIDE8		"MUST MOVE OF ONE PAD"
#define TXT_AIDE9		"ARROUND YOU AND DESTROY"
#define TXT_AIDE10		"A PAD WHERE YOU WANT"
#define TXT_AIDE11		"GAME OPTIONS:"
#define TXT_AIDE12		"CLIC ON THE GREEN WORDS"
#define TXT_AIDE13		"PLAYERS CAN BE HUMAN OR COM"
#define TXT_AIDE14		"PUTER WITH 3 LEVELS OF AI"
#define TXT_AIDE15		"METEORITS: A PAD IS"
#define TXT_AIDE16		"DESTROYED EACH TURN OF PLAY"
#define TXT_INTEMP		"METEORITS:"
#define TXT_DEPLACE		"MOVE:"
#define TXT_BOULEFEU	"FIREBALL:"
#define TXT_VITESSEJEU	"GAME SPEED:"
#define TXT_TAILLEGRILLE "GRID SIZE:  X"
#define TXT_J1GAGNE		"PLAYER 1 WINS"
#define TXT_J2GAGNE		"PLAYER 2 WINS"
#define TXT_J1PLACE		"PLAYER 1: CHOOSE A PAD"
#define TXT_J2PLACE		"PLAYER 2: CHOOSE A PAD"
#define TXT_J1DEPLACE	"PLAYER 1: MOVE"
#define TXT_J2DEPLACE	"PLAYER 2: MOVE"
#define TXT_J1TROU		"PLAYER 1: FIRE"
#define TXT_J2TROU		"PLAYER 2: FIRE"

// Fichiers des images
#define IMG_MENU		"menu.jpg"
#define IMG_FOND1		"fond1.jpg"
#define IMG_FOND2		"fond2.jpg"
#define IMG_FOND3		"fond3.jpg"
#define IMG_FOND4		"fond4.jpg"
#define IMG_FOND5		"fond5.jpg"
#define IMG_NIVO		"dossi-nivo.png"
#define IMG_POLICE1		"police-jaune.png"
#define IMG_POLICE2		"police-verte.png"
#define IMG_POLICE3		"police-rouge.png"
#define IMG_CASE		"case.png"
#define IMG_FEU			"feu.png"
#define IMG_J1COURS		"dossi-cours.png"
#define IMG_J2COURS		"dossa-cours.png"
#define IMG_J1LANCE		"dossi-lance.png"
#define IMG_J2LANCE		"dossa-lance.png"
#define IMG_J1GAGNE		"dossi-victoire.png"
#define IMG_J2GAGNE		"dossa-victoire.png"
#define IMG_J1PERDU		"dossi-ange.png"
#define IMG_J2PERDU		"dossa-ange.png"
#define IMG_J1ATTEND	"dossi-attend.png"
#define IMG_J2ATTEND	"dossa-attend.png"
#define IMG_J1ATTENDFEU	"dossi-attend-feu.png"
#define IMG_J2ATTENDFEU	"dossa-attend-feu.png"

// Vitesse d'affichage des animations en frames par seconde
int FPS;

// Location des images
char imgpath [128];
char fichier [160];

// Types pour d�signer le contenu des cases, l'action en cours, ...
typedef enum {NON, OUI} BOOL;
typedef enum {VIDE, J1, J2, TROU} CASE;
typedef enum {J1_DEPLACE, J1_TROU, J2_DEPLACE, J2_TROU} ACTION;
typedef enum {HUMAIN, ORDI} TYPEJOUEUR;
typedef enum {COURSB, COURSH, COURSG, COURSD, LANCE, GAGNE, PERDU, ATTEND, ATTENDFEU} ANIM;
typedef enum {RIEN, QUITTER, MENU, REFAIRE, CONSEIL} CHXMENU;

// Structure contenant une police de caractere
typedef struct {
	SDL_Rect rect;				// Rectangle de la taille d'un caract�re
	SDL_Surface *img;			// Image contenant les caract�res
} POLICE;

// Structure permettant de m�moriser les cases offrant le plus de d�placements
typedef struct {
  int x, y;						// Coordonn�es de la case
  int nb;						// Valeur de la case
} MAX;

// Structure contenant les infos d'un joueur
typedef struct {
	CASE n;						// Num�ro du joueur
	TYPEJOUEUR type;			// Humain ou Ordi
	int nivo;					// Niveau du joueur
	int x, y;					// Position sur la grille
	SDL_Rect rect, src;			// Rectangle de position sur l'�cran et dans l'image d'animation
	SDL_Surface *imgCours;		// Images des d�placements
	SDL_Surface *imgLance;		// Images du lancement de la boule de feu
	SDL_Surface *imgGagne;		// Images de la victoire
	SDL_Surface *imgPerdu;		// Images de la d�faite
	SDL_Surface *imgAttend;		// Images de l'attente du d�placement
	SDL_Surface *imgAttendFeu;	// Images de l'attente du lancement de la boule de feu
	SDL_Surface *imgFeu;		// Images de la boule de feu
	// Nombre d'images pour les animations
	int nCours, nLance, nGagne, nPerdu, nAttend, nAttendFeu, nFeu;
	ANIM anim;					// Animation en cours
	int frame;					// Image en cours dans l'animation
} JOUEUR;

// Structure contenant les infos sur la grille du jeu
typedef struct {
	int xc, yc;					// Nombre de cases en longueur et en hauteur
	int x1, y1;					// Position du coin haut gauche de la grille
	int x2, y2;					// Position du coin bas  droite de la grille
	int tc;						// Taille des cases
	int c[XCASEMAX][YCASEMAX];	// Grille indiquant le contenu des cases
	SDL_Surface *imgCase;		// Image d'une case
	SDL_Surface *imgFond;		// Image de l'arri�re plan
} GRILLE;

// Structure contenant les infos g�n�rales sur le jeu
typedef struct {
	SDL_Surface *ecran;			// Surface graphique principale (fenetre du prog)
	SDL_Surface *back;			// Surface graphique tampon (back buffer)
	SDL_Rect titre;				// Rectangle o� se situe le texte indiquant l'action en cours
	SDL_Rect rMenu[4];			// Rects des boutons du menu pendant le jeu
	POLICE police1, police2, police3;	// Polices pour l'affichage de caract�res
	BOOL J1ROUGE;				// Le joueur 1 est vert ou rouge ?
	BOOL INTEMP;				// Pluie de boule de feu ?
	BOOL DOUBLE_DEPLACE;		// Deplacement simple ou double ?
	BOOL DOUBLE_BOULEFEU;		// Place un ou deux trous ?
	TYPEJOUEUR J1TYPE;			// Humain ou Ordi ?
	TYPEJOUEUR J2TYPE;			// Humain ou Ordi ?
	int J1NIVO;					// Niveau de l'IA
	int J2NIVO;					// Niveau de l'IA
	int XGRILLE, YGRILLE;		// Taille de la grille
} JEU;

// Renvoie au hasard un nombre compris entre les valeurs pass�es en arguments (la 2e non incluse)
int Alea (short, short);

// Affiche l'erreur obtenue pendant le chargement d'une image
void ImageErreur (char *);

// Convertit les coordonn�es du clic de souris en position dans la grille
int Case2CoordX (int, GRILLE);
int Case2CoordY (int, GRILLE);

// Convertit la position dans la grille en coordonn�es sur l'�cran
int Clic2CaseX (int, GRILLE);
int Clic2CaseY (int, GRILLE);

// Synchronise le temps sur la vitesse d'affichage (FPS)
void Attend_FPS (void);

// Fait trembler l'�cran
void Secouer_Ecran (JOUEUR[], GRILLE, SDL_Surface *);

// Affiche le joueur avec l'image de l'animation en cours
void Afficher_Joueur (JOUEUR *, SDL_Surface *);

// Renvoie OUI si le trou a put �tre plac� (et place le trou)
BOOL Placer_Trou (int , int , JOUEUR[], int, GRILLE *, JEU *, BOOL, CHXMENU *);

// Renvoie OUI si le joueur a put �tre d�plac� (et d�place le joueur)
BOOL Deplacer_Joueur (int, int, JOUEUR[], int, GRILLE *, JEU *, BOOL, CHXMENU *);

// Place le joueur � la position choisie et renvoie OUI si la position est disponible
BOOL Placer_Joueur (int, int, JOUEUR[], int, GRILLE *, JEU *, BOOL, CHXMENU *);

// Permet aux joueurs de choisir leur position de d�part
void Placer_Joueurs (JEU *, JOUEUR[], GRILLE *, CHXMENU *);

// Cr�e la grille, met les images en m�moire, ...
void Constructeur (JOUEUR[], GRILLE *, BOOL, SDL_Surface *, SDL_Surface *);

// Lib�re la m�moire occup�e par les images
void Destructeur (JOUEUR[], GRILLE *);

// Pr�vois les possibilit�s de d�placements pour chaques cases
void Calcul_Choix (GRILLE , int[][], int , int *);

// Cr�e une grille � partir du jeu pour calculer les d�placements possibles
void Meilleur_Choix (GRILLE , int[][], int, JOUEUR[], int, MAX[], MAX []);

// Place sur l'une des meilleurs cases
void Ordi_Place (int *, int *, JOUEUR, GRILLE);

// D�place sur l'une des meilleures cases
void Ordi_Deplace (int *, int *, JOUEUR [], int, GRILLE);

// D�truit l'une des meilleures cases de l'adversaire
void Ordi_Trou (int *, int *, JOUEUR [], int, GRILLE);

// Boucle de jeu principale qui g�re l'affichage et les �v�nements (souris, ...)
BOOL Jeu (JEU *);

// Affiche un caract�re � l'�cran
void Afficher_Caractere (int, int, char, POLICE, SDL_Surface *);

// Affiche une chaine de caract�re en faisant apparaitre les caract�res en
// glissan de l'un des bord de l'�cran
BOOL Afficher_Chaine (int, int, int, char[], POLICE, SDL_Surface *, SDL_Surface *);

// Affiche les images des personnages repr�sentant les niveaux d'intelligence
void Afficher_IA (int, SDL_Rect[], SDL_Rect[], SDL_Surface *, SDL_Surface *, SDL_Surface *, SDL_Surface *);

// Menu de choix des options
BOOL Menu (JEU *);

// Fonction principale qui v�rifie les param�tres pass�s � l'appel du programme,
// initialise la biblioth�que graphique SDL, cr�e l'affichage, ...
int main (int, char *[]);
