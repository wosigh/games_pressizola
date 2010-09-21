#include <dirent.h>
#include <sys/param.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <pthread.h>
#include "isola.h"
#include "resize.h"

// ****************************************************************************
// ********************************** Main ************************************
// ****************************************************************************
int main (int argc, char *argv[])
{
	JEU jeu;					// Données principales du jeu
	Uint32 flags;				// Paramêtres pour l'affichage (fenètre ou plein écran)
	int i;
	int audio_rate = 44100;
	Uint16 audio_format = MIX_DEFAULT_FORMAT;
	int audio_channels = 2;
	int audio_buffers = 1536;
	
	srand ((unsigned) time (NULL));	// Initialise le générateur de nbs aléatoires
	//flags = SDL_SWSURFACE;			// Parametre de l'écran
	flags = SDL_FULLSCREEN;
	music = NULL;
	musicArray = NULL;
	musicArraySize = 0;
	songCount = 0;
	getcwd( appHome, MAXPATHLEN );
	
	//Populate the array of songs
	pthread_create( &musicThread, NULL, dirsearcher, ".mp3" );

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

	strcpy (imgpath, appHome);
	strcat (imgpath, "/images/" );
	strcpy (sfxpath, appHome);
	strcat (sfxpath, "/sounds/" );

	strcat( boomloc, sfxpath );
	strcat( boomloc, SFX_BOOM );
	strcat( wooshloc, sfxpath );
	strcat( wooshloc, SFX_WOOSH );
	strcat( victoryloc, sfxpath );
	strcat( victoryloc, SFX_VICTORY );
	strcat( lossloc, sfxpath );
	strcat( lossloc, SFX_LOSS );
	
	//printf( "BOOMLOC: %s\nWOOSHLOC: %s\nVICTORLOC: %s\nLOSSLOC: %s\n", boomloc, wooshloc, victoryloc, lossloc );

	// Initialise SDL
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{	fprintf (stderr, "Impossible d'initialiser SDL (%s)\n", SDL_GetError());
		exit (1);
	}

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
	{
		printf("Unable to open audio!\n");
		exit(1);
	}

	// Libérera les ressources utilisées par SDL en quittant
	atexit (SDL_Quit);
	
	// Initialise l'affichage en plein écran ou en fenêtre en 1024x768x32
	if (!(jeu.ecran = SDL_SetVideoMode (RES_X, RES_Y, 16, flags)))
	{	fprintf (stderr, "Impossible de definir le mode 900*600 (%s)\n", SDL_GetError());
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
	//jeu.police1big.img = SDL_ConvertSurface(jeu.police1.img, jeu.police1.img->format, SDL_SWSURFACE);
	jeu.police2big.img = SDL_ConvertSurface(jeu.police2.img, jeu.police2.img->format, SDL_SWSURFACE);
	jeu.police3big.img = SDL_ConvertSurface(jeu.police3.img, jeu.police3.img->format, SDL_SWSURFACE);
	//jeu.police1big.img = SDL_ResizeFactor(jeu.police1big.img, TXT_BIG, IMG_RES_FILTER);
	jeu.police2big.img = SDL_ResizeFactor(jeu.police2big.img, TXT_BIG, IMG_RES_FILTER);
	jeu.police3big.img = SDL_ResizeFactor(jeu.police3big.img, TXT_BIG, IMG_RES_FILTER);
	SDL_SetColorKey (jeu.police1.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police1.img->format, 255, 0, 255));
	SDL_SetColorKey (jeu.police2.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police2.img->format, 255, 0, 255));
	SDL_SetColorKey (jeu.police3.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police3.img->format, 255, 0, 255));
	//SDL_SetColorKey (jeu.police1big.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police1big.img->format, 255, 0, 255));
	SDL_SetColorKey (jeu.police2big.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police2big.img->format, 255, 0, 255));
	SDL_SetColorKey (jeu.police3big.img, SDL_SRCCOLORKEY, SDL_MapRGB (jeu.police3big.img->format, 255, 0, 255));
	jeu.police1.rect.x = jeu.police2.rect.x = jeu.police3.rect.x = 0;
	jeu.police1big.rect.x = jeu.police2big.rect.x = jeu.police3big.rect.x = 0;
	jeu.police1.rect.y = jeu.police2.rect.y = jeu.police3.rect.y = 0;
	jeu.police1big.rect.y = jeu.police2big.rect.y = jeu.police3big.rect.y = 0;
	jeu.police1.rect.w = jeu.police2.rect.w = jeu.police3.rect.w = jeu.police1.img->w / 37;		// Il y a 37 caracteres dans la police
	jeu.police1big.rect.w = jeu.police2big.rect.w = jeu.police3big.rect.w = jeu.police3big.img->w / 37;
	jeu.police1.rect.h = jeu.police2.rect.h = jeu.police3.rect.h = jeu.police1.img->h;
	jeu.police1big.rect.h = jeu.police2big.rect.h = jeu.police3big.rect.h = jeu.police3big.img->h;
	
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
	musicVol = SND_LOW;
	sfxVol = SND_MED;
	
	boomChannel = -1;
	boom = Mix_LoadWAV(boomloc);
	wooshChannel = -1;
	woosh = Mix_LoadWAV(wooshloc);
	victoryChannel = -1;
	victory = Mix_LoadWAV(victoryloc);
	lossChannel = -1;
	loss = Mix_LoadWAV(lossloc);

	setSFXVol();

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
	SDL_FreeSurface (jeu.police1big.img);
	SDL_FreeSurface (jeu.police2big.img);
	SDL_FreeSurface (jeu.police3big.img);
	
	Mix_CloseAudio();
	
	return 0;
}
