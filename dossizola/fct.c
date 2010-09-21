#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "isola.h"

#include <dirent.h>
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>


// ****************************************************************************
// ********************************** Alea ************************************
// ****************************************************************************
int Alea (short min, short max)
{
	return (unsigned short)rand() * (max - min) / 65536 + min;
}


// ****************************************************************************
// ****************************** ImageErreur *********************************
// ****************************************************************************
void ImageErreur (char *image)
{
	// Affiche un message d'erreur et quitte le programme
	fprintf (stderr, "Impossible de charger l'image %s (%s)\n", image, SDL_GetError ());
	exit (1);
}


// ****************************************************************************
// ****************************** Case2Coord **********************************
// ****************************************************************************
int Case2CoordX (int x, GRILLE grille)
{
	// Renvoit l'abscisse écran du coin haut gauche de la case
	return grille.x1 + x * grille.tc;
}

int Case2CoordY (int y, GRILLE grille)
{
	// Renvoit l'abscisse écran du coin haut gauche de la case
	return grille.y1 + y * grille.tc;
}


// ****************************************************************************
// ****************************** Clic2Case ***********************************
// ****************************************************************************
int Clic2CaseX (int x, GRILLE grille)
{
	// Renvoit l'abscisse de la case cliquée ou -1 si en dehors de la grille
	if (x >= grille.x1 && x < grille.x2)
		return (x - grille.x1 - 1) / grille.tc;
	else
		return -1;
}

int Clic2CaseY (int y, GRILLE grille)
{
	// Renvoit l'ordonnée de la case cliquée ou -1 si en dehors de la grille
	if (y >= grille.y1 && y <= grille.y2)
		return (y - grille.y1 - 1) / grille.tc;
	else
		return -1;
}


// ****************************************************************************
// ******************************* Attendre_FPS *******************************
// ****************************************************************************
void Attendre_FPS (void)
{
	static int temps = 0;				// Moment du dernier appel de cette fonction
	
	// Marque une pause pour synchroniser l'affichage sur la vitesse choisie pour que
	// le temps entre 2 appels de cette fonction soit de: 1000 ms / Frames Par Seconde
	while (SDL_GetTicks() < temps + 1000 / FPS);
	
	temps = SDL_GetTicks ();			// Moment où cette fonction se termine
}


// ****************************************************************************
// ********************************* Dans_Rect ********************************
// ****************************************************************************
BOOL Dans_Rect (int x, int y, SDL_Rect rect)
{
	if (x >= rect.x && x <= (rect.x + rect.w) && y >= rect.y && y <= (rect.y + rect.h))
		return OUI;
	else
		return NON;
}


// ****************************************************************************
// ************************** Afficher_Caractere ******************************
// ****************************************************************************
void Afficher_Caractere (int x, int y, char c, POLICE police, SDL_Surface *ecran)
{
	SDL_Rect dest;

	// Place le rectangle sur la bonne image du caractère à afficher
	if (c >= 'A' && c <= 'Z') police.rect.x = (c - 'A') * police.rect.w;
	else if (c >= 'a' && c <= 'z') police.rect.x = (c - 'a') * police.rect.w;
	else if (c >= '0' && c <= '9') police.rect.x = (c - '0' + 26) * police.rect.w;
	else if (c == ':') police.rect.x = (36 * police.rect.w);
	else return;

	// Position sur l'écran
	dest.x = x;
	dest.y = y;
	dest.w = police.rect.w;
	dest.h = police.rect.h;

	// Copie le caractère et met à jour
	SDL_BlitSurface (police.img, &police.rect, ecran, &dest);
	SDL_UpdateRects (ecran, 1, &dest);
}


// ****************************************************************************
// **************************** Afficher_Chaine *******************************
// ****************************************************************************
BOOL Afficher_Chaine (int x, int y, int vitesse, char chaine[], POLICE police, SDL_Surface *ecran, SDL_Surface *back)
{
	SDL_Rect dest = {0, 0, police.rect.w, police.rect.h};
	SDL_Event evt;
	int xDep, yDep, xDest, yDest;
	int c;
	int frame;

	// Parcours la chaine de caractère et affiche les caractères un par un en
	// les faisant glisser de l'un des bord de l'écran
	for (c = 0; chaine[c] != '\0'; c ++)
	{
		// Choisit aléatoirement le bord de l'écran et sa position dessus
		switch (Alea (0, 2))
		{
			case 0:	xDep = Alea (0, ecran->w - police.rect.w);
					yDep = Alea (0, 2) ? 0 : ecran->h - police.rect.h;
					break;
			case 1:	xDep = Alea (0, 2) ? 0 : ecran->w - police.rect.w;
					yDep = Alea (0, ecran->h - police.rect.h);
					break;
			default:	xDep = yDep = 0;
		}
		
		xDest = x + (c * police.rect.w);
		yDest = y;
		
		// Fait glisser le caractère du bord de l'écran vers sa position finale
		if (vitesse > 0)
		for (frame = 0; frame <= vitesse; frame ++)
		{
			// Quitte sur un appui sur la touche [ESC]
			if (SDL_PollEvent (&evt) && evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) return OUI;
		
			// Efface l'ancienne position
			SDL_BlitSurface (back, &dest, ecran, &dest);
			
			// Affiche le caractère au bon endroit
			Afficher_Caractere (xDep + ((xDest - xDep) * frame) / vitesse, yDep + ((yDest - yDep) * frame) / vitesse, chaine[c], police, ecran);
			SDL_UpdateRects (ecran, 1, &dest);
			
			// Fait la moyenne pour savoir où doit être affiché le caractère
			dest.x = xDep + ((xDest - xDep) * frame) / vitesse;
			dest.y = yDep + ((yDest - yDep) * frame) / vitesse;
			Attendre_FPS ();
		}

		// Affiche le caractère à sa position finale
		Afficher_Caractere (xDest, yDest, chaine[c], police, ecran);
		Afficher_Caractere (xDest, yDest, chaine[c], police, back);
	}
	return NON;
}

// ****************************************************************************
// ******************************* Questionner ********************************
// ****************************************************************************
BOOL Questionner (char ch[], POLICE police1, POLICE police2, POLICE police3, SDL_Surface *ecran)
{
	SDL_Surface *save, *noire;
	SDL_Event evt;
	SDL_Rect box;			// Rect de copie de la "boite de dialogue"
	SDL_Rect oui, non;		// Rects de cliquage des boutons OUI et NON
	
	// Rect de la boite de dialogue du message
	box.w = police1.rect.w * (strlen (ch) + 3);
	box.h = police1.rect.h * 5;
	box.x = (ecran->w - box.w) / 2;
	box.y = (ecran->h - box.h) / 2 + police1.rect.h / 3;
	
	// Crée une surface pour sauvegarder la partie de l'écran qui sera recouverte par le message
	save = SDL_CreateRGBSurface (SDL_SWSURFACE, box.w, box.h, 32, 0, 0, 0, 0);
	SDL_BlitSurface (ecran, &box, save, NULL);
	
	// Crée une surface noire semi-transparente qui servira à assombrir une partie de l'écran
	if (!(noire = SDL_DisplayFormat (save)))
	{ 	fprintf (stderr, "Impossible de créer un back buffer (%s)\n", SDL_GetError ());
		SDL_FreeSurface (save);
		SDL_FreeSurface (noire);
		exit (1);
	}
	SDL_FillRect (noire, NULL, SDL_MapRGB (noire->format, 0, 0, 0));
	SDL_SetAlpha (noire, SDL_SRCALPHA, 128);
	
	// Assombrit l'écran et met à jour
	SDL_BlitSurface (noire, NULL, ecran, &box);
	SDL_UpdateRects (ecran, 1, &box);
	SDL_FreeSurface (noire);
	
	// Rects des boutons OUI et NON
	oui.w = strlen (TXT_OUI) * police3.rect.w;
	oui.h = police3.rect.h;
	oui.x = ecran->w / 2 + police3.rect.w;
	oui.y = ecran->h / 2 + police3.rect.h;
	
	non.w = strlen (TXT_NON) * police2.rect.w;
	non.h = police2.rect.h;
	non.x = ecran->w / 2 - non.w - police2.rect.w;
	non.y = ecran->h / 2 + police2.rect.h;
	
	// Affiche le texte de la question et les textes des boutons OUI et NON
	Afficher_Chaine ((ecran->w - strlen (ch) * police1.rect.w) / 2, ecran->h / 2 - police1.rect.h, 0, ch, police1, ecran, ecran);
	Afficher_Chaine (oui.x, oui.y, 0, TXT_OUI, police3, ecran, ecran);
	Afficher_Chaine (non.x, non.y, 0, TXT_NON, police2, ecran, ecran);
	
	while (1)
	if (SDL_PollEvent (&evt))
	{	
		// Anulle sur un appui de la touche [ESC], ou un clic sur NON
		if ((evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) || (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1 && Dans_Rect (evt.button.x, evt.button.y, non)))
		{	SDL_BlitSurface (save, NULL, ecran, &box);
			SDL_UpdateRects (ecran, 1, &box);
			SDL_FreeSurface (save);
			return NON;
		}
		
		// Retourne OUI à la fonction ayant posé cette question
		if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1 && Dans_Rect (evt.button.x, evt.button.y, oui))
		{	SDL_BlitSurface (save, NULL, ecran, &box);
			SDL_UpdateRects (ecran, 1, &box);
			SDL_FreeSurface (save);
			return OUI;
		}
	}
}


// ****************************************************************************
// ******************************** Informer **********************************
// ****************************************************************************
BOOL Informer (char ch[], POLICE police, SDL_Surface *ecran)
{
	SDL_Rect box;			// Rect de copie de la "boite de dialogue"
	SDL_Surface *noire;
	
	// Rect de la boite de dialogue du message
	box.w = police.rect.w * (strlen (ch) + 3);
	box.h = police.rect.h * 3;
	box.x = (ecran->w - box.w) / 2;
	box.y = (ecran->h - box.h) / 2;
	
	// Crée une surface noire semi-transparente qui servira à assombrir une partie de l'écran
	if (!(noire = SDL_CreateRGBSurface (SDL_SWSURFACE, box.w, box.h, 32, 0, 0, 0, 0)))
	{ 	fprintf (stderr, "Impossible de créer une surface (%s)\n", SDL_GetError ());
		SDL_FreeSurface (noire);
		exit (1);
	}
	SDL_FillRect (noire, NULL, SDL_MapRGB (noire->format, 0, 0, 0));
	SDL_SetAlpha (noire, SDL_SRCALPHA, 128);
	
	// Assombrit l'écran et met à jour
	SDL_BlitSurface (noire, NULL, ecran, &box);
	SDL_UpdateRects (ecran, 1, &box);
	SDL_FreeSurface (noire);
	
	// Affiche le texte de la question et les textes des boutons OUI et NON
	Afficher_Chaine ((ecran->w - strlen (ch) * police.rect.w) / 2, (ecran->h - police.rect.h) / 2, 0, ch, police, ecran, ecran);
}

// ****************************************************************************
// ********************************** Sound ***********************************
// ****************************************************************************
void startMusic ( char *songpath )
{
	if ( songCount > 0 )
	{
		if ( strcmp( songpath, "random") == 0 )
		{
			int songChoice;
			songChoice = Alea (1, songCount);
			songpath = musicArray[songChoice].songpath;
			printf( "playing song %s from %s\n", musicArray[songChoice].songname, musicArray[songChoice].songpath );
		}
	} else {
		strcpy( songpath, sfxpath );
		strcat( songpath, SNG_INTRO );
	}
	music = Mix_LoadMUS( songpath );
	if(!music) {
		printf("Mix_LoadMUS(\"%s\"): %s\n", songpath, Mix_GetError());
		// this might be a critical error...
	}
	Mix_VolumeMusic(musicVol);
	Mix_PlayMusic(music, 0);
	Mix_HookMusicFinished(musicDone);
}

void stopMusic()
{
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music = NULL;
}

void toggleMusic ()
{
	char *songname;
	switch (musicVol)
	{
		case SND_OFF: musicVol = SND_LOW; strcpy( songpath, "random" ); startMusic( songpath ); break;
		case SND_LOW: musicVol = SND_MED; Mix_VolumeMusic(musicVol); break;
		case SND_MED: musicVol = SND_HIGH; Mix_VolumeMusic(musicVol); break;
		case SND_HIGH: musicVol = SND_OFF; stopMusic(); break;
	}
}

void musicDone() {
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music = NULL;
	startMusic( "random" );
}

void toggleSFX ()
{
	switch (sfxVol)
	{
		case SND_OFF: sfxVol = SND_LOW; setSFXVol(); break;
		case SND_LOW: sfxVol = SND_MED; setSFXVol(); break;
		case SND_MED: sfxVol = SND_HIGH; setSFXVol(); break;
		case SND_HIGH: sfxVol = SND_OFF; setSFXVol(); break;
	}
}

void setSFXVol ()
{
	Mix_VolumeChunk(boom, sfxVol/2);
	Mix_VolumeChunk(woosh, sfxVol);
	Mix_VolumeChunk(victory, sfxVol);
	Mix_VolumeChunk(loss, sfxVol);
}

// ****************************************************************************
// ******************************* Key Handler ******************************** 
// ****************************************************************************
void handleKey( SDL_KeyboardEvent key )
{
	switch(key.keysym.sym)
	{
		case SDLK_m:
		if(key.state == SDL_RELEASED)
		{
			toggleMusic();
		}
		break;

		case SDLK_s:
		if(key.state == SDL_RELEASED)
		{
			toggleSFX();
		}
		break;
	}
}

// ****************************************************************************
// ******************************* Dir Walker ********************************* 
// ****************************************************************************
int walker( char *searching, char *result ) {
	struct dirent *dir;
	DIR *subd;
	subd = opendir( "." );
	if( subd == NULL )
	{
		return 1;
	}
	while( ( dir = readdir( subd ) ) ) {
		if( strcmp( dir->d_name, "." ) == 0 || //ignore current folder symlink
			strcmp( dir->d_name, ".." ) == 0 || //ignore previous folder symlink
			strncmp( dir->d_name, ".", 1 ) == 0 || //ignore hidden folders
			strcmp( dir->d_name, "ringtones" ) == 0 ) //ignore ringtones folder
		{
			continue;
		}
		if( dir->d_type == DT_DIR )
		{
			chdir( dir->d_name );
			walker( searching, result );
			chdir( ".." );
			continue;
		} else {
			if ((strlen(dir->d_name) >= 4) && !strncasecmp(dir->d_name+strlen(dir->d_name)-4, searching, 4))
			{
				int  len;
				getcwd( result, MAXPATHLEN );
				len = strlen( result );
				snprintf( result + len, MAXPATHLEN - len, "/%s", dir->d_name );
				SONG temp;
				temp.songname = malloc((strlen(dir->d_name) + 1) * sizeof(char));
				strncpy(temp.songname, dir->d_name, strlen(dir->d_name) + 1);
				temp.songpath = malloc((strlen(result) + 1) * sizeof(char));
				strncpy(temp.songpath, result, strlen(result) + 1);
				if (AddToMusicArray(temp) == -1)
					return 1;
			}
		}
	}
	closedir( subd );
	return *result == 0;
}

void *dirsearcher( char *string ) {
	DIR *d;
	char buf[MAXPATHLEN] = { 0 };
	d = opendir( MUSIC_LOC );
	if ( chdir( MUSIC_LOC ) == 0 )
	{
		if( walker( string, buf ) == 0 )
		{
			printf( "Found %d songs in %s\n", songCount, MUSIC_LOC );
		} else {
			puts( "No songs found" );
		}
	} else {
		printf( "Couldn't chdir to %s\n", MUSIC_LOC );
	}
	chdir( appHome );
	closedir( d );
}

int AddToMusicArray( SONG item )
{
        if(songCount == musicArraySize)
        {
                if (musicArraySize == 0)
                        musicArraySize = 100;
                else
                        musicArraySize += 100;
                void *_tmp = realloc(musicArray, (musicArraySize * sizeof(SONG)));
                if (!_tmp)
                {
                        fprintf(stderr, "ERROR: Couldn't realloc memory!\n");
                        return(-1);
                }
                musicArray = (SONG*)_tmp;
        }
        musicArray[songCount] = item;
        songCount++;
        return songCount;
}
