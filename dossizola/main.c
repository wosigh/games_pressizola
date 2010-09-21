#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "isola.h"


// ****************************************************************************
// ********************************** Main ************************************
// ****************************************************************************
int main (int argc, char *argv[])
{
	JEU jeu;					// Données principales du jeu
	Uint32 flags;				// Paramêtres pour l'affichage (fenètre ou plein écran)
	int i;
	
	srand ((unsigned) time (NULL));	// Initialise le générateur de nbs aléatoires
	flags = SDL_SWSURFACE;			// Parametre de l'écran
	
	// Lit les options passées en argument au programme
	for (i = 1; i < argc; i ++)
	{
		if (!strcmp (argv[i], "-h") || !strcmp (argv[i], "--help"))		// Affiche une aide
		{	printf ("\n************** Do'SSi Zo'la 1.0 **************\n");
			printf ("\n\t\t-h : this help\n\t\t-f : fullscreen\n\t\t-w : windowed\n");
			printf ("\ncontact the authors:\nCyril CLEMENT  <dossiman@users.sourceforge.net>\nGaelle DAIREAUX <ponstyl@users.sourceforge.net>\n");
			printf ("\nhome page: http://dossizola.sourceforge.net\n\n");
			exit (0);
		}
		else if (!strcmp (argv[i], "-f"))	// Mode plein écran
			flags = SDL_HWSURFACE | SDL_FULLSCREEN;
		else if (!strcmp (argv[i], "-w"))	// Mode fenétrée
			flags = SDL_HWSURFACE;
		else
		{	fprintf (stderr, "%s not allowed.\n", argv[i]);
			exit (1);
		}
	}	
	strcpy (imgpath, argv[0]);
	for (i = strlen (imgpath); i >= 0; i --)
	{	
		if (imgpath[i] == '/')
		{	imgpath[i] = '\0';
			strcat (imgpath, "/images/");
			break;
		}
		if (!i) strcpy (imgpath, "./images/");
	}
	
	// Initialise SDL
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{	fprintf (stderr, "Impossible d'initialiser SDL (%s)\n", SDL_GetError());
		exit (1);
	}
	// Libérera les ressources utilisées par SDL en quittant
	atexit (SDL_Quit);
	
	// Initialise l'affichage en plein écran ou en fenêtre en 1024x768x32
	if (!(jeu.ecran = SDL_SetVideoMode (800, 600, 32, flags)))
		if (!(jeu.ecran = SDL_SetVideoMode (800, 600, 24, flags)))
			if (!(jeu.ecran = SDL_SetVideoMode (800, 600, 16, flags)))
			{	fprintf (stderr, "Impossible de definir le mode 800*600 (%s)\n", SDL_GetError());
				exit (1);
			}
	// Définit le titre de la fenètre
	SDL_WM_SetCaption ("Do'SSi Zo'la", "Do'SSi Zo'la");
	
	// Crée un back buffer (une copie de sauvegarde de l'écran en mémoire)
	if (!(jeu.back = SDL_DisplayFormat (jeu.ecran)))
	{ 	fprintf (stderr, "Impossible de créer un back buffer (%s)\n", SDL_GetError ());
		SDL_FreeSurface (jeu.back);
		exit (1);
	}
	
	// Prépare la police de caractère
	sprintf (fichier, "%s%s", imgpath, IMG_POLICE1);
	if (!(jeu.police1.img = IMG_Load (fichier))) ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_POLICE2);
	if (!(jeu.police2.img = IMG_Load (fichier))) ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_POLICE3);
	if (!(jeu.police3.img = IMG_Load (fichier))) ImageErreur (fichier);
	SDL_SetColorKey (jeu.police1.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police1.img->format, 255, 0, 255));
	SDL_SetColorKey (jeu.police2.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police2.img->format, 255, 0, 255));
	SDL_SetColorKey (jeu.police3.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police3.img->format, 255, 0, 255));
	jeu.police1.rect.x = jeu.police2.rect.x = jeu.police3.rect.x = 0;
	jeu.police1.rect.y = jeu.police2.rect.y = jeu.police3.rect.y = 0;
	jeu.police1.rect.w = jeu.police2.rect.w = jeu.police3.rect.w = jeu.police1.img->w / 37;		// Il y a 37 caracteres dans la police
	jeu.police1.rect.h = jeu.police2.rect.h = jeu.police3.rect.h = jeu.police1.img->h;
	
	// Paramètres par défaut
	jeu.J1ROUGE = DFT_J1ROUGE;
	jeu.J1TYPE = DFT_J1TYPE;
	jeu.J2TYPE = DFT_J2TYPE;
	jeu.J1NIVO = DFT_J1NIVO;
	jeu.J2NIVO = DFT_J2NIVO;
	jeu.INTEMP = DFT_INTEMP;
	jeu.DOUBLE_DEPLACE  = DFT_DOUBLE_DEPLACE;
	jeu.DOUBLE_BOULEFEU = DFT_DOUBLE_BOULEFEU;
	jeu.XGRILLE = DFT_XGRILLE;
	jeu.YGRILLE = DFT_YGRILLE;
	FPS = DFT_FPS;
	
	while (1)
	{
		// Lance le menu de choix des options du jeu si la fonction retourne oui, le prog quitte
		if (Menu (&jeu)) break;
		
		// Lance une partie
		if (Jeu (&jeu)) break;
	}
	
	printf ("\nIf you have fun with this game, do not forget to rate it at :\n\thttp://happypenguin.org/show?Do'SSi%%20Zo'la\n\nIf you have some question or remark, please send mail to:\n\tdossiman@users.sourceforge.net\n\n");
	
	SDL_FreeSurface (jeu.back);
	SDL_FreeSurface (jeu.police1.img);
	SDL_FreeSurface (jeu.police2.img);
	SDL_FreeSurface (jeu.police3.img);
	
	return 0;
}
