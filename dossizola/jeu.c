#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "isola.h"

// ****************************************************************************
// ******************************* Tester_Menu ********************************
// ****************************************************************************
CHXMENU Tester_Menu (SDL_Event *evt, BOOL fQuitter, BOOL fMenu, BOOL fRefaire, BOOL fConseil, BOOL fMusic, JEU *jeu)
{
	int i;
	int x, y;
	
	// Demande de quitter le jeu
	if (evt->type == SDL_QUIT || (evt->type == SDL_KEYDOWN && evt->key.keysym.sym == SDLK_ESCAPE))
		if (Questionner (TXT_QSTMENU, jeu->police1, jeu->police2, jeu->police3, jeu->ecran))
			return MENU;
	
	// Place le curseur aux bonnes coordonnées
	if (evt->type == SDL_MOUSEMOTION)
	{	x = evt->motion.x;
		y = evt->motion.y;
	}
	else if (evt->type == SDL_MOUSEBUTTONDOWN)
	{	x = evt->button.x;
		y = evt->button.y;
	}
	else return RIEN;
	
	// Réaffiche les textes en alpha blending !
	for (i = 0; i < 5; i ++)
		SDL_BlitSurface (jeu->back, &jeu->rMenu[i], jeu->ecran, &jeu->rMenu[i]);
	
	// Affiche en opaque le texte sous le curseur
	if (fQuitter && Dans_Rect (x, y, jeu->rMenu[0]))
		Afficher_Chaine (jeu->rMenu[0].x, jeu->rMenu[0].y, 0, TXT_QUITTER, jeu->police3, jeu->ecran, jeu->ecran);
	if (fMenu && Dans_Rect (x, y, jeu->rMenu[1]))
		Afficher_Chaine (jeu->rMenu[1].x, jeu->rMenu[1].y, 0, TXT_MENU,    jeu->police3, jeu->ecran, jeu->ecran);
	if (fRefaire && Dans_Rect (x, y, jeu->rMenu[2]))
		Afficher_Chaine (jeu->rMenu[2].x, jeu->rMenu[2].y, 0, TXT_REFAIRE, jeu->police3, jeu->ecran, jeu->ecran);
	if (fConseil && Dans_Rect (x, y, jeu->rMenu[3]))
		Afficher_Chaine (jeu->rMenu[3].x, jeu->rMenu[3].y, 0, TXT_CONSEIL, jeu->police3, jeu->ecran, jeu->ecran);
	if (fMusic && Dans_Rect (x, y, jeu->rMenu[4]))
		Afficher_Chaine (jeu->rMenu[4].x, jeu->rMenu[4].y, 0, TXT_MUSIC, jeu->police3, jeu->ecran, jeu->ecran);
	
	SDL_UpdateRects (jeu->ecran, 5, jeu->rMenu);
	
	if (evt->type == SDL_MOUSEBUTTONDOWN && evt->button.button == 1)		// Clic bouton gauche
	{
		if (fQuitter && Dans_Rect (x, y, jeu->rMenu[0]))
			if (Questionner (TXT_QSTQUITTER, jeu->police1, jeu->police2, jeu->police3, jeu->ecran))
				return QUITTER;
		if (fMenu && Dans_Rect (x, y, jeu->rMenu[1]))
			if (Questionner (TXT_QSTMENU, jeu->police1, jeu->police2, jeu->police3, jeu->ecran))
				return MENU;
		if (fRefaire && Dans_Rect (x, y, jeu->rMenu[2]))
			if (Questionner (TXT_QSTREFAIRE, jeu->police1, jeu->police2, jeu->police3, jeu->ecran))
			{	// Message d'attente
				Informer (TXT_PATIENTEZ, jeu->police1, jeu->ecran);
				return REFAIRE;
			}
		if (fConseil && Dans_Rect (x, y, jeu->rMenu[3]))
			return CONSEIL;
		if (fMusic && Dans_Rect (x, y, jeu->rMenu[4]))
			toggleMusic();
		
		SDL_GetMouseState (&x, &y);
		evt->type = SDL_MOUSEMOTION;
		evt->motion.x = x;
		evt->motion.y = y;
		SDL_PeepEvents (evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
	}
	
	return RIEN;
}


// ****************************************************************************
// ***************************** Afficher_Joueur ******************************
// ****************************************************************************
void Afficher_Joueur (JOUEUR *j, SDL_Surface *ecran)
{	
	SDL_Surface *img;		// Surface contenant l'animation en cours
	int n;					// Nombre d'images de l'animation en cours
	
	// Si l'animation est l'un des 4 déplacements
	if (j->anim >= COURSB && j->anim <= COURSD)
	{	
		// Place le rectangle source à la bonne position en fonction de l'image à afficher dans l'animation
		j->src.w = j->imgCours->w / j->nCours;
		j->src.h = j->imgCours->h / 4;	
		j->src.x = j->src.w * j->frame;
		j->src.y = j->src.h * j->anim;
		SDL_BlitSurface (j->imgCours, &j->src, ecran, &j->rect);
		return;
	}
	
	// Autre type d'animation
	else
	{
		if (j->anim == LANCE)	{ img = j->imgLance;	n = j->nLance; }
		if (j->anim == GAGNE)	{ img = j->imgGagne;	n = j->nGagne; }
		if (j->anim == PERDU)	{ img = j->imgPerdu;	n = j->nPerdu; }
		if (j->anim == ATTEND)	{ img = j->imgAttend;	n = j->nAttend; }
		if (j->anim == ATTENDFEU) { img = j->imgAttendFeu; n = j->nAttendFeu; }
		
		// Place le rectangle source à la bonne position en fonction de l'image à afficher dans l'animation
		j->src.w = img->w / n;
		j->src.h = img->h;	
		j->src.x = j->src.w * j->frame;
		j->src.y = 0;
		SDL_BlitSurface (img, &j->src, ecran, &j->rect);
	}
}


// ****************************************************************************
// ****************************** Secouer_Ecran *******************************
// ****************************************************************************
void Secouer_Ecran (JOUEUR j[], GRILLE grille, SDL_Surface *ecran)
{
	SDL_Rect fond, rect;
	int xDecal, yDecal;
	int i;
	boomChannel = Mix_PlayChannel(-1, boom, 0);

	// Rectangle de copie du fond d'écran
	fond.x = grille.x1 - (j[0].imgCours->w / j[0].nCours - grille.imgCase->w) / 2 - 16;
	fond.y = grille.y1 - (j[0].imgCours->h / 4           - grille.imgCase->h) / 2 - 16;
	fond.w = grille.x2 - grille.x1 + 64;
	fond.h = grille.y2 - grille.y1 + 64;
	// Rectangle de copie des cases
	rect.w = rect.h = grille.tc;
	
	// Déplace aléatoirement la grille
	for (i = 0; i <= TREMBLE_NB / TREMBLE_VITESSE; i ++)
	{
		// Décallage aléatoire
		xDecal = Alea (-TREMBLE_DECAL, TREMBLE_DECAL);
		yDecal = Alea (-TREMBLE_DECAL, TREMBLE_DECAL);
		// Replace la grille à la position de départ
		if (i == TREMBLE_NB / TREMBLE_VITESSE) xDecal = yDecal = 0;
		
		// Affiche le fond
		SDL_BlitSurface (grille.imgFond, &fond, ecran, &fond);
		
		// Affiche les cases là où il y en a, avec le décallage du tremblement
		for (rect.x = grille.x1 + xDecal; rect.x < grille.x2 + xDecal; rect.x += rect.w)
			for (rect.y = grille.y1 + yDecal; rect.y < grille.y2 + yDecal; rect.y += rect.h)
				if (grille.c[(rect.x-xDecal-grille.x1)/rect.w][(rect.y-yDecal-grille.y1)/rect.h] != TROU)
					SDL_BlitSurface (grille.imgCase, NULL, ecran, &rect);
		
		// Affiche les joueurs avec le décallage du tremblement
		j[0].rect.x += xDecal;
		j[0].rect.y += yDecal;
		j[1].rect.x += xDecal;
		j[1].rect.y += yDecal;
		
		Afficher_Joueur (j[0].y <= j[1].y ? &j[0] : &j[1], ecran);
		Afficher_Joueur (j[0].y <= j[1].y ? &j[1] : &j[0], ecran);
		
		// Rétablit les positions des joueurs sans le tremblement
		j[0].rect.x -= xDecal;
		j[0].rect.y -= yDecal;
		j[1].rect.x -= xDecal;
		j[1].rect.y -= yDecal;
		
		Attendre_FPS ();
		SDL_UpdateRects (ecran, 1, &fond);
	}
}


// ****************************************************************************
// ******************************** Intemperie ********************************
// ****************************************************************************
BOOL Intemperie (JOUEUR j[], GRILLE *grille, JEU *jeu, CHXMENU *chxmenu)
{
	SDL_Rect src, dest, prec;	// Rectangles pour la copie d'image
	int xDep, yDep;				// Coordonnées où la boule décolle
	int xDest, yDest;			// Coordonnées où la boule s'écrase
	int x, y;					// Case à détruire
	int n;						// Anime à utiliser (celle du j1 ou du j2)
	int frame = 0;				// Image courante dans l'animation de la boule
	int alpha;					// Transparence de l'animation
	BOOL monte = OUI;			// La boule de feu monte ou descend ?
	SDL_Event evt;				// Pour détecter un appui sur [ESC]
	int xSouris, ySouris;		// Coords de la souris
	*chxmenu = RIEN;
	
	// Choisit aléatoirement une case de la grille
	x = Alea (0, grille->xc);
	y = Alea (0, grille->yc);
	
	// Vérifie s'il l'on peut placer le trou
	if (grille->c[x][y] != VIDE) return NON;
	grille->c[x][y] = TROU;
	
	// Choisit aléatoirement une animation
	n = Alea (0, 2);
	if (n != 0 || n != 1) n = 0;
	
	// Choisit aléatoirement le bord de l'écran d'où sera lancé la boule de feu
	switch (Alea (0, 2))
	{
		case 0:	xDep = Alea (0, jeu->ecran->w - j[n].imgFeu->w / j[n].nFeu);
				yDep = Alea (0, 2) ? 0 : jeu->ecran->h - j[n].imgFeu->h;
				break;
		case 1:	xDep = Alea (0, 2) ? 0 : jeu->ecran->w - j[n].imgFeu->w / j[n].nFeu;
				yDep = Alea (0, jeu->ecran->h - j[n].imgFeu->h);
				break;
		default:	xDep = yDep = 0;
	}
	
	// La boule de feu est centrée au centre de la case à détruire à l'arrivée
	xDest = Case2CoordX (x, *grille) - (j[n].imgFeu->w / j[n].nFeu - grille->imgCase->w) / 2;
	yDest = Case2CoordY (y, *grille) - (j[n].imgFeu->h - grille->imgCase->h) / 2;
	
	// Rectangle sur l'image dans la séquence d'animation de la boule de feu
	src.w = dest.w = j[n].imgFeu->w / j[n].nFeu;
	src.h = dest.h = j[n].imgFeu->h;
	src.x = src.y = 0;
	dest.x = dest.y = 0;
	wooshChannel = Mix_PlayChannel(-1, woosh, 0);
	// Boucle de l'animation tant que toutes l'animation de la boule de feu n'a pas été faite
	do
	{
		if (SDL_PollEvent (&evt))		// Attend un événement
		{	// Teste si le curseur passe sur l'un des boutons du menu, ou si on clique dessus, ou si on veut quitter
			*chxmenu = Tester_Menu (&evt, OUI, OUI, OUI, NON, OUI, jeu);
			if (*chxmenu) return OUI;
		}
		else
		{	// Fait déplacer la souris s'il n'y a pas eu d'événements
			SDL_GetMouseState (&xSouris, &ySouris);
			evt.type = SDL_MOUSEMOTION;
			evt.motion.x = xSouris;
			evt.motion.y = ySouris;
			SDL_PeepEvents (&evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
		}

		// Réaffiche le décor sur l'ancienne position
		SDL_BlitSurface (jeu->back, &dest, jeu->ecran, &dest);
		
		// Affiche les joueurs avec le plus en avant par dessus l'autre
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[n]  : &j[!n], jeu->ecran);
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[!n] : &j[n],  jeu->ecran);
		
		// Diminue la transparence de la boule
		if (frame <= j[n].nFeu) alpha = 255 - 191 * frame / j[n].nFeu;
		SDL_SetAlpha (j[n].imgFeu, SDL_SRCALPHA, alpha);
		
		// Copie l'image de la boule de feu sur l'écran
		prec = dest;
		dest.x = xDep + frame * (xDest - xDep) / (j[n].nFeu * 2);
		dest.y = yDep + frame * (yDest - yDep) / (j[n].nFeu * 2);
		SDL_BlitSurface (j[n].imgFeu, &src, jeu->ecran, &dest);
		
		// La boule monte ou descend ? affiche l'image suivante ou précedente dans l'animation
		if (monte) src.x += src.w * BOULE_VITESSE;
		else       src.x -= src.w * BOULE_VITESSE;
		if (src.x >= j[n].nFeu * src.w - src.w * BOULE_VITESSE) monte = NON;
		
		// Attend d'être synchronisé sur la vitesse d'affichage et rafraîchit l'écran
		Attendre_FPS ();
		SDL_UpdateRects (jeu->ecran, 1, &dest);
		SDL_UpdateRects (jeu->ecran, 1, &prec);
	}

	while (++ frame <= j[n].nFeu*2);
	
	// Rectangle du trou crée
	dest.w = dest.h = grille->imgCase->w;
	dest.x = Case2CoordX (x, *grille);
	dest.y = Case2CoordY (y, *grille);
	
	// Copie l'image du trou sur l'écran et dans le back buffer
	SDL_BlitSurface (grille->imgFond, &dest, jeu->ecran, &dest);
	SDL_BlitSurface (grille->imgFond, &dest, jeu->back,  &dest);
	SDL_UpdateRects (jeu->ecran, 1, &dest);
	
	// Réaffiche les joueurs
	Afficher_Joueur (j[n].y <= j[!n].y ? &j[n]  : &j[!n], jeu->ecran);
	Afficher_Joueur (j[n].y <= j[!n].y ? &j[!n] : &j[n],  jeu->ecran);
	SDL_UpdateRects (jeu->ecran, 1, &dest);
	SDL_SetAlpha (j[n].imgFeu, SDL_SRCALPHA, 191);
	
	// Fait trembler l'écran
	Secouer_Ecran (j, *grille, jeu->ecran);
	
	return OUI;
}


// ****************************************************************************
// ******************************** Placer_Trou ********************************
// ****************************************************************************
BOOL Placer_Trou (int x, int y, JOUEUR j[], int n, GRILLE *grille, JEU *jeu, BOOL conseil, CHXMENU *chxmenu)
{
	SDL_Rect src, dest, prec;	// Rectangles pour la copie d'image
	int xDep, yDep;				// Coordonnées où la boule décolle
	int xDest, yDest;			// Coordonnées où la boule s'écrase
	float xDecal, yDecal;		// Unité de déplacement de la boule
	float xPos, yPos;			// Position de la boule
	int frame;					// Image courante dans l'animation de la boule
	BOOL monte = OUI;			// La boule de feu monte ou descend ?
	BOOL bis = OUI;				// Répète la même image d'animation
	SDL_Event evt;				// Pour détecter un appui sur [ESC]
	int xSouris, ySouris;		// Coords de la souris
	*chxmenu = RIEN;

	// Vérifie s'il l'on peut placer le trou
	if (grille->c[x][y] != VIDE) return NON;
	if (!conseil) grille->c[x][y] = TROU;
	
	// Efface l'affichage de l'action en cours
	SDL_BlitSurface (jeu->back, &jeu->titre, jeu->ecran, &jeu->titre);
	SDL_UpdateRects (jeu->ecran, 1, &jeu->titre);
	
	// Si un conseil est demandé, l'affiche et rend le joueur et la boule transparents
	if (conseil)
	{	Afficher_Chaine ((jeu->ecran->w - strlen (TXT_CONSEIL) * jeu->police1.rect.w) / 2, jeu->titre.y, 0, TXT_CONSEIL, jeu->police1, jeu->ecran, jeu->ecran);
		SDL_SetAlpha (j[n].imgLance, SDL_SRCALPHA, 128);
		SDL_SetAlpha (j[n].imgFeu, SDL_SRCALPHA, 128);
	}
	
	// Animation du joueur qui lance la boule
	j[n].anim = LANCE;
	j[n].frame = 0;
	frame = -30;
	
	// La boule de feu est centrée au centre de la case à détruire à l'arrivée
	xDest = Case2CoordX (x, *grille) - (j[n].imgFeu->w / j[n].nFeu - grille->imgCase->w) / 2;
	yDest = Case2CoordY (y, *grille) - (j[n].imgFeu->h - grille->imgCase->h) / 2;
	// La boule de feu est centrée sur le point du joueur au départ
	xDep = Case2CoordX (j[n].x, *grille) - (j[n].imgFeu->w / j[n].nFeu - grille->imgCase->w) / 2 - 24;
	yDep = Case2CoordY (j[n].y, *grille) - (j[n].imgFeu->h - grille->imgCase->h) / 2 - 16;
	// Rectangle sur l'image dans la séquence d'animation de la boule de feu
	src.w = dest.w = j[n].imgFeu->w / j[n].nFeu;
	src.h = dest.h = j[n].imgFeu->h;
	src.x = src.y = 0;
	dest.x = xPos = xDep;
	dest.y = yPos = yDep;
	xDecal = (float)(xDest - xDep) / (float)(j[n].nFeu * 2 - 1);
	yDecal = (float)(yDest - yDep) / (float)(j[n].nFeu * 2 - 1);

	wooshChannel = Mix_PlayChannel(-1, woosh, 0);
	// Boucle de l'animation tant que toutes l'animation de la boule de feu n'a pas été faite
	do
	{
		if (SDL_PollEvent (&evt))		// Attend un événement
		{	// Teste si le curseur passe sur l'un des boutons du menu, ou si on clique dessus, ou si on veut quitter
			*chxmenu = Tester_Menu (&evt, OUI, OUI, OUI, NON, OUI, jeu);
			if (*chxmenu) return OUI;
		}
		else
		{	// Fait déplacer la souris s'il n'y a pas eu d'événements
			SDL_GetMouseState (&xSouris, &ySouris);
			evt.type = SDL_MOUSEMOTION;
			evt.motion.x = xSouris;
			evt.motion.y = ySouris;
			SDL_PeepEvents (&evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
		}
		
		// Réaffiche le décor sur les anciennes positions
		SDL_BlitSurface (jeu->back, &dest, jeu->ecran, &dest);
		SDL_BlitSurface (jeu->back, &j[n].rect, jeu->ecran, &j[n].rect);
		
		// Passe à l'image suivante dans l'animation du joueur (avec une pause entre les frames 0 et 20)
		if ((frame < 0 || frame > 20) && j[n].frame < j[n].nLance - 1)
			if (bis = !bis) j[n].frame ++;
		
		// Affiche les joueurs avec le plus en avant par dessus l'autre
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[n]  : &j[!n], jeu->ecran);
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[!n] : &j[n],  jeu->ecran);
		
		// Anime la boule à partir de la frame 0 (on a commencé à -30)
		if (frame > 0)
		{
			// Copie l'image de la boule de feu sur l'écran
			prec = dest;
			if (frame < 30)
			{	xPos += xDecal/2.0;
				yPos += yDecal/2.0;
			}
			else if (frame < 45)
			{	xPos += xDecal;
				yPos += yDecal;
			}
			else
			{	xPos += xDecal*2.0;
				yPos += yDecal*2.0;
			}
			dest.x = (int) xPos;
			dest.y = (int) yPos;
			SDL_BlitSurface (j[n].imgFeu, &src, jeu->ecran, &dest);
			
			// La boule monte ou descend ? affiche l'image suivante ou précedente dans l'animation
			if (monte) src.x += src.w;
			else       src.x -= src.w;
			if (src.x >= j[n].nFeu * src.w - src.w) monte = NON;
		}
		
		// Attend d'être synchronisé sur la vitesse d'affichage et rafraîchit l'écran
		Attendre_FPS ();
		SDL_UpdateRects (jeu->ecran, 1, &j[n].rect);
		if (frame > 0)
		{	SDL_UpdateRects (jeu->ecran, 1, &dest);
			SDL_UpdateRects (jeu->ecran, 1, &prec);
		}
		
	}
	while (++ frame < j[n].nFeu * 2);
	
	// Si un conseil était demandé remet les animations à leur transparence d'origine et efface la boule
	if (conseil)
	{	SDL_SetAlpha (j[n].imgLance, SDL_SRCALPHA, 255);
		SDL_SetAlpha (j[n].imgFeu, SDL_SRCALPHA, 191);
		SDL_BlitSurface (jeu->back, &dest, jeu->ecran, &dest);
		SDL_BlitSurface (jeu->back, &j[n].rect, jeu->ecran, &j[n].rect);
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[n]  : &j[!n], jeu->ecran);
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[!n] : &j[n],  jeu->ecran);
		SDL_UpdateRects (jeu->ecran, 1, &dest);
	}
	else
	{
		// Rectangle du trou crée
		dest.w = dest.h = grille->imgCase->w;
		dest.x = Case2CoordX (x, *grille);
		dest.y = Case2CoordY (y, *grille);
		
		// Copie l'image du trou sur l'écran et dans le back buffer
		SDL_BlitSurface (grille->imgFond, &dest, jeu->ecran, &dest);
		SDL_BlitSurface (grille->imgFond, &dest, jeu->back,  &dest);
		SDL_UpdateRects (jeu->ecran, 1, &dest);
		
		// Réaffiche les joueurs
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[n]  : &j[!n], jeu->ecran);
		Afficher_Joueur (j[n].y <= j[!n].y ? &j[!n] : &j[n],  jeu->ecran);
		SDL_UpdateRects (jeu->ecran, 1, &dest);
		
		// Fait trembler l'écran
		Secouer_Ecran (j, *grille, jeu->ecran);
	}
	
	return OUI;
}


// ****************************************************************************
// ****************************** Deplacer_Joueur *****************************
// ****************************************************************************
BOOL Deplacer_Joueur (int x, int y, JOUEUR j[], int n, GRILLE *grille, JEU *jeu, BOOL conseil, CHXMENU *chxmenu)
{
	SDL_Rect prec;				// Rectangle de l'ancienne position du joueur (à effacer)
	SDL_Rect save, src;			// Rectangles de l'ancienne position du joueur (à sauvegarder pour un conseil)
	SDL_Event evt;				// Pour détecter un appui sur [ESC]
	BOOL bis = OUI;				// Répète la même image d'animation
	int xSouris, ySouris;		// Coords de la souris
	int frame;
	*chxmenu = RIEN;
	
	// Vérifie si l'on peut déplacer le joueur, le déplacement autorisé est d'une case autour de sa position
	if (grille->c[x][y] != VIDE ||  j[n].x < x-1 || j[n].x > x+1 || j[n].y < y-1 || j[n].y > y+1) return NON;
	
	// Efface l'affichage de l'action en cours
	SDL_BlitSurface (jeu->back, &jeu->titre, jeu->ecran, &jeu->titre);
	SDL_UpdateRects (jeu->ecran, 1, &jeu->titre);
	
	// Choisit la bonne animation pour le joueur
	if (j[n].x > x) j[n].anim = COURSG;
	if (j[n].y > y) j[n].anim = COURSH;
	if (j[n].x < x) j[n].anim = COURSD;
	if (j[n].y < y) j[n].anim = COURSB;
	j[n].frame = 0;
	
	// Si c'est un conseil qui est demandé, sauvegarde la position du joueur et affiche le titre "CONSEIL"
	if (conseil)
	{	save = j[n].rect;
		src.x = src.y = 0;
		src.w = j[n].src.w;
		src.h = j[n].src.h;
		Afficher_Chaine ((jeu->ecran->w - strlen (TXT_CONSEIL) * jeu->police1.rect.w) / 2, jeu->titre.y, 0, TXT_CONSEIL, jeu->police1, jeu->ecran, jeu->ecran);
	}
	
	// Animation du joueur qui court d'une case à l'autre
	for (frame = 0; frame < grille->imgCase->w; frame += 2)
	{
		if (SDL_PollEvent (&evt))		// Attend un événement
		{	// Teste si le curseur passe sur l'un des boutons du menu, ou si on clique dessus, ou si on veut quitter
			*chxmenu = Tester_Menu (&evt, OUI, OUI, OUI, NON, OUI, jeu);
			if (*chxmenu) return OUI;
		}
		else
		{	// Fait déplacer la souris s'il n'y a pas eu d'événements
			SDL_GetMouseState (&xSouris, &ySouris);
			evt.type = SDL_MOUSEMOTION;
			evt.motion.x = xSouris;
			evt.motion.y = ySouris;
			SDL_PeepEvents (&evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
		}
		
		// Efface l'ancienne position du joueur
		SDL_BlitSurface (jeu->back, &j[n].rect, jeu->ecran, &j[n].rect);
		
		// Place le rectangle source sur l'image en cours dans l'animation
		j[n].src.x = j[n].frame * j[n].imgCours->w / j[n].nCours;
		
		// Déplace le rectangle destination sur l'écran selon la direction où se déplace le joueur
		prec = j[n].rect;
		if (j[n].x > x) j[n].rect.x -= 2;
		if (j[n].y > y) j[n].rect.y -= 2;
		if (j[n].x < x) j[n].rect.x += 2;
		if (j[n].y < y) j[n].rect.y += 2;
		
		// Affiche les joueurs avec le plus en avant par dessus l'autre
		if (conseil)
		{	SDL_BlitSurface (jeu->back, &save, jeu->ecran, &save);
			SDL_BlitSurface (j[n].imgCours, &src, jeu->ecran, &save);
			SDL_SetAlpha (j[n].imgCours, SDL_SRCALPHA, 128);
		}
		Afficher_Joueur (j[n].y < j[!n].y ? &j[n]  : &j[!n], jeu->ecran);
		Afficher_Joueur (j[n].y < j[!n].y ? &j[!n] : &j[n],  jeu->ecran);
		if (conseil)
		{	SDL_SetAlpha (j[n].imgCours, SDL_SRCALPHA, 255);
		}
		
		// Attend d'être synchronisé sur la vitesse d'affichage et rafraîchit l'écran
		Attendre_FPS ();
		SDL_UpdateRects (jeu->ecran, 1, &j[n].rect);
		SDL_UpdateRects (jeu->ecran, 1, &prec);
		if (conseil) SDL_UpdateRects (jeu->ecran, 1, &save);
		
		// Passe à l'image suivante dans l'animation où répète la même (reboucle si terminé)
		if (bis = !bis) j[n].frame ++;
		if (j[n].frame >= j[n].nCours) j[n].frame = 0;
	}
	
	if (conseil)
	{	// Restaure le joueur à sa position
		SDL_BlitSurface (jeu->back, &j[n].rect, jeu->ecran, &j[n].rect);
		Afficher_Joueur (&j[!n], jeu->ecran);
		SDL_UpdateRects (jeu->ecran, 1, &j[n].rect);
		j[n].rect = save;
	}
	else
	{	// Libère la place anciennement occupée par le joueur
		grille->c[j[n].x][j[n].y] = VIDE;
		// Modifie ses coordonnées
		j[n].x = x;
		j[n].y = y;
		// Et entre sa nouvelle position dans la grille
		grille->c[j[n].x][j[n].y] = j[n].n;
	}
	return OUI;
}


// ****************************************************************************
// ******************************* Placer_Joueur ******************************
// ****************************************************************************
BOOL Placer_Joueur (int x, int y, JOUEUR j[], int n, GRILLE *grille, JEU *jeu, BOOL conseil, CHXMENU *chxmenu)
{
	int alpha;					// Valeur de la transparence de l'image du joueur
	SDL_Event evt;
	int xSouris, ySouris;		// Coords de la souris
	*chxmenu = RIEN;

	// Vérifie qu'il y a de la place pour placer le joueur et le place
	if (grille->c[x][y] != VIDE) return NON;
	
	// Coordonnées du joueur dans la grille
	if (!conseil) grille->c[x][y] = j[n].n;
	j[n].x = x;
	j[n].y = y;
	
	// Coordonnées du joueur sur l'écran
	j[n].rect.x = Case2CoordX (j[n].x, *grille) - (j[n].imgCours->w / j[n].nCours - grille->imgCase->w) / 2;
	j[n].rect.y = Case2CoordY (j[n].y, *grille) - (j[n].imgCours->h / 4           - grille->imgCase->h) / 2;
	
	// Le joueur apparaît progressivement (en diminuant sa transparence)
	for (alpha = 0; alpha <= 250; alpha += 10)
	{
		if (SDL_PollEvent (&evt))
		{	// Teste si le curseur passe sur l'un des boutons du menu, ou si on clique dessus, ou si on veut quitter
			*chxmenu = Tester_Menu (&evt, OUI, OUI, OUI, NON, OUI, jeu);
			if (*chxmenu) return OUI;
		}
		else
		{	// Fait déplacer la souris s'il n'y a pas eu d'événements
			SDL_GetMouseState (&xSouris, &ySouris);
			evt.type = SDL_MOUSEMOTION;
			evt.motion.x = xSouris;
			evt.motion.y = ySouris;
			SDL_PeepEvents (&evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
		}
		
		// Règle la transparence du joueur et l'affiche
		SDL_SetAlpha (j[n].imgCours, SDL_SRCALPHA, alpha);
		SDL_BlitSurface (jeu->back, &j[n].rect, jeu->ecran, &j[n].rect);
		
		// Si c'est au tour du 2e joueur, on réaffiche le premier
		if (n == 1) Afficher_Joueur (&j[0], jeu->ecran);
		Afficher_Joueur (&j[n], jeu->ecran);
		
		// Attend d'être synchronisé sur la vitesse d'affichage et rafraîchit l'écran
		Attendre_FPS ();
		SDL_UpdateRects (jeu->ecran, 1, &j[n].rect);
	}
	
	if (conseil)
	{	SDL_BlitSurface (jeu->back, &j[n].rect, jeu->ecran, &j[n].rect);
		SDL_UpdateRects (jeu->ecran, 1, &j[n].rect);
	}
	
	return OUI;
}


// ****************************************************************************
// ******************************** Placer_Joueurs ****************************
// ****************************************************************************
void Placer_Joueurs (JEU *jeu, JOUEUR j[], GRILLE *grille, CHXMENU *chxmenu)
{
	SDL_Event evt;				// Récupère les infos d'un événement
	int tour = 0;				// Tour du joueur pour se placer
	int x, y;					// Coordonnées du clic de souris
	*chxmenu = RIEN;
	
	while (1)
	{
		// Placement d'un joueur dirigé par l'ordinateur
		if (j[tour].type == ORDI)
		{	
			Ordi_Place (&x, &y, j[tour], *grille);
			Placer_Joueur (x, y, j, tour, grille, jeu, NON, chxmenu);
			if (*chxmenu) return;
			
			// Incrémente le tour et quitte si tous les joueurs ont été placés
			if (++ tour == 2) return;
			continue;
		}
		
		if (SDL_PollEvent (&evt))
		{
			// Affiche l'action en cours
			SDL_BlitSurface (jeu->back, &jeu->titre, jeu->ecran, &jeu->titre);
			if (tour == 0) Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J1PLACE) * jeu->police2.rect.w) / 2, jeu->titre.y, 0, TXT_J1PLACE, jeu->J1ROUGE ? jeu->police3 : jeu->police2, jeu->ecran, jeu->ecran);
			if (tour == 1) Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J2PLACE) * jeu->police2.rect.w) / 2, jeu->titre.y, 0, TXT_J2PLACE, jeu->J1ROUGE ? jeu->police2 : jeu->police3, jeu->ecran, jeu->ecran);
			SDL_UpdateRects (jeu->ecran, 1, &jeu->titre);
			
			if (evt.type == SDL_KEYUP)
			{
				handleKey (evt.key);
			}

			if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1)	// Bouton gauche enfoncé
			{
				x = Clic2CaseX (evt.button.x, *grille);		// Récupère les coords
				y = Clic2CaseY (evt.button.y, *grille);		// de la case cliquée
				
				// S'il s'agit bien d'une case de la grille
				if (x != -1 && y != -1)
				// Si le joueur a put être placé
				if (Placer_Joueur (x, y, j, tour, grille, jeu, NON, chxmenu))
					// Incrémente le tour et quitte si tous les joueurs ont été placés
					if (++ tour == 2) return;
			}
			
			// Teste si le curseur passe sur l'un des boutons du menu, ou si on clique dessus, ou si on veut quitter
			*chxmenu = Tester_Menu (&evt, OUI, OUI, OUI, OUI, OUI, jeu);
			if (*chxmenu == CONSEIL)
			{	Ordi_Place (&x, &y, j[tour], *grille);
				Placer_Joueur (x, y, j, tour, grille, jeu, OUI, chxmenu);
				if (*chxmenu) return;
			}
			else if (*chxmenu) return;
			//if (*chxmenu != RIEN && *chxmenu != CONSEIL) return;
		}
		else
		{	// Fait déplacer la souris s'il n'y a pas eu d'événements
			SDL_GetMouseState (&x, &y);
			evt.type = SDL_MOUSEMOTION;
			evt.motion.x = x;
			evt.motion.y = y;
			SDL_PeepEvents (&evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
		}
	}
}


// ****************************************************************************
// ****************************** Constructeur ********************************
// ****************************************************************************
void Constructeur (JOUEUR j[], GRILLE *grille, BOOL c, SDL_Surface *ecran, SDL_Surface *back)
{
	SDL_Surface *img;			// Surface contenant temporairement une image
	SDL_Rect src, dest;			// Rectangles de copie d'image
	int x, y;
	
	// Créer une surface qui contiendra l'arrière plan
	if (!(grille->imgFond = SDL_DisplayFormat (ecran)))
	{	fprintf (stderr, "Impossible de créer le fond d'écran (%s)\n", SDL_GetError ());
		SDL_FreeSurface (grille->imgFond);
		exit (1);
	}
	
	// Charge l'image de fond ...
	switch (Alea (1, 6))
	{
		case 1:	sprintf (fichier, "%s%s", imgpath, IMG_FOND1);	break;
		case 2:	sprintf (fichier, "%s%s", imgpath, IMG_FOND2);	break;
		case 3:	sprintf (fichier, "%s%s", imgpath, IMG_FOND3);	break;
		case 4:	sprintf (fichier, "%s%s", imgpath, IMG_FOND4);	break;
		case 5:	sprintf (fichier, "%s%s", imgpath, IMG_FOND5);	break;
		default: sprintf (fichier, "%s%s", imgpath, IMG_FOND1);	break;
	}
	if (!(img = IMG_Load (fichier))) ImageErreur (fichier);
	
	// ... et l'affiche en mosaique dans l'arrière plan
	src.x = src.y = 0;
	
	for (dest.y = 0; dest.y < grille->imgFond->h; dest.y += dest.h)
		for (dest.x = 0; dest.x < grille->imgFond->w; dest.x += dest.w)
		{	
			src.w = dest.w = img->w;
			src.h = dest.h = img->h;
			SDL_BlitSurface (img, &src, grille->imgFond, &dest);
		}
	
	// Libère l'image de fond et copie l'arrière plan dans le back buffer
	SDL_FreeSurface (img);
	SDL_BlitSurface (grille->imgFond, NULL, back, NULL);
	
	// Charge les images dans des surfaces et teste si elles sont présentes
	/*img = SDL_CreateRGBSurface (SDL_HWSURFACE | SDL_SRCALPHA, 3840, 128, 32, 0xff, 0x00ff, 0x0000ff, 0x000000ff);
	IMG_InvertAlpha (0);
	if (!(img = IMG_Load (IMG_FEU))) ImageErreur (IMG_FEU);
	j[c].imgFeu = SDL_CreateRGBSurface (SDL_HWSURFACE | SDL_SRCALPHA, img->w, img->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	j[!c].imgFeu = SDL_CreateRGBSurface (SDL_HWSURFACE | SDL_SRCALPHA, img->w, img->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	SDL_BlitSurface (img, NULL, j[c].imgFeu,  NULL);
	SDL_BlitSurface (img, NULL, j[!c].imgFeu, NULL);
	SDL_FreeSurface (img);*/
	/*  Add in 5 tile images then uncomment this code chunk
	switch (Alea (1, 6))
	{
		case 1:	sprintf (fichier, "%s%s", imgpath, IMG_CASE1);	break;
		case 2:	sprintf (fichier, "%s%s", imgpath, IMG_CASE2);	break;
		case 3:	sprintf (fichier, "%s%s", imgpath, IMG_CASE3);	break;
		case 4:	sprintf (fichier, "%s%s", imgpath, IMG_CASE4);	break;
		case 5:	sprintf (fichier, "%s%s", imgpath, IMG_CASE5);	break;
		default: sprintf (fichier, "%s%s", imgpath, IMG_CASE1);	break;
	} And comment/delete the line below*/
	sprintf (fichier, "%s%s", imgpath, IMG_CASE);
	if (!(grille->imgCase	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_FEU);
	if (!(j[c].imgFeu		= IMG_Load (fichier)))	ImageErreur (fichier);
	if (!(j[!c].imgFeu		= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J1COURS);
	if (!(j[c].imgCours		= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J2COURS);
	if (!(j[!c].imgCours	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J1LANCE);
	if (!(j[c].imgLance		= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J2LANCE);
	if (!(j[!c].imgLance	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J1GAGNE);
	if (!(j[c].imgGagne		= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J2GAGNE);
	if (!(j[!c].imgGagne	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J1PERDU);
	if (!(j[c].imgPerdu		= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J2PERDU);
	if (!(j[!c].imgPerdu	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J1ATTEND);
	if (!(j[c].imgAttend	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J2ATTEND);
	if (!(j[!c].imgAttend	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J1ATTENDFEU);
	if (!(j[c].imgAttendFeu	= IMG_Load (fichier)))	ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J2ATTENDFEU);
	if (!(j[!c].imgAttendFeu = IMG_Load (fichier))) ImageErreur (fichier);
	
	// Définit le nombres d'images pour les animations
	j[c].nFeu       = j[!c].nFeu       = 30;
	j[c].nCours     = j[!c].nCours     = 16;
	j[c].nLance     = j[!c].nLance     = 18;
	j[c].nGagne     = j[!c].nGagne     = 35;
	j[c].nPerdu     = j[!c].nPerdu     = 20;
	j[c].nAttend    = j[!c].nAttend    = 30;
	j[c].nAttendFeu = j[!c].nAttendFeu = 20;
	
	// Animation courante et image à afficher
	j[c].anim = j[!c].anim = COURSB;
	j[c].frame = j[!c].frame = 0;
	
	// Définit la couleur qui détermine la transparence (le violet)
	SDL_SetColorKey (j[c].imgFeu,	 	SDL_SRCCOLORKEY, SDL_MapRGB (j[c].imgFeu->format,		255, 0, 255));
	SDL_SetColorKey (j[!c].imgFeu,	 	SDL_SRCCOLORKEY, SDL_MapRGB (j[!c].imgFeu->format,		255, 0, 255));
	SDL_SetColorKey (j[c].imgCours,	 	SDL_SRCCOLORKEY, SDL_MapRGB (j[c].imgCours->format,		255, 0, 255));
	SDL_SetColorKey (j[!c].imgCours, 	SDL_SRCCOLORKEY, SDL_MapRGB (j[!c].imgCours->format,	255, 0, 255));
	SDL_SetColorKey (j[c].imgLance,	 	SDL_SRCCOLORKEY, SDL_MapRGB (j[c].imgLance->format,		255, 0, 255));
	SDL_SetColorKey (j[!c].imgLance, 	SDL_SRCCOLORKEY, SDL_MapRGB (j[!c].imgLance->format,	255, 0, 255));
	SDL_SetColorKey (j[c].imgGagne,	 	SDL_SRCCOLORKEY, SDL_MapRGB (j[c].imgGagne->format,		255, 0, 255));
	SDL_SetColorKey (j[!c].imgGagne, 	SDL_SRCCOLORKEY, SDL_MapRGB (j[!c].imgGagne->format,	255, 0, 255));
	SDL_SetColorKey (j[c].imgPerdu,	 	SDL_SRCCOLORKEY, SDL_MapRGB (j[c].imgPerdu->format,		255, 0, 255));
	SDL_SetColorKey (j[!c].imgPerdu, 	SDL_SRCCOLORKEY, SDL_MapRGB (j[!c].imgPerdu->format,	255, 0, 255));
	SDL_SetColorKey (j[c].imgAttend, 	SDL_SRCCOLORKEY, SDL_MapRGB (j[c].imgAttend->format,	255, 0, 255));
	SDL_SetColorKey (j[!c].imgAttend, 	SDL_SRCCOLORKEY, SDL_MapRGB (j[!c].imgAttend->format,	255, 0, 255));
	SDL_SetColorKey (j[c].imgAttendFeu,	SDL_SRCCOLORKEY, SDL_MapRGB (j[c].imgAttendFeu->format,	255, 0, 255));
	SDL_SetColorKey (j[!c].imgAttendFeu, SDL_SRCCOLORKEY, SDL_MapRGB (j[!c].imgAttendFeu->format, 255, 0, 255));
	
	// Définit la transparence des cases et de la boule de feu
	SDL_SetAlpha (grille->imgCase,	SDL_SRCALPHA, 155);
	SDL_SetAlpha (j[c].imgFeu,		SDL_SRCALPHA, 191);
	SDL_SetAlpha (j[!c].imgFeu,		SDL_SRCALPHA, 191);
	SDL_SetAlpha (j[c].imgPerdu,	SDL_SRCALPHA, 155);
	SDL_SetAlpha (j[!c].imgPerdu,	SDL_SRCALPHA, 155);
		
	// Définit la taille des rectangles des joueurs
	j[c].rect.w = j[c].imgCours->w / j[c].nCours;
	j[c].rect.h = j[c].imgCours->h / 4;
	j[!c].rect.w = j[!c].imgCours->w / j[c].nCours;
	j[!c].rect.h = j[!c].imgCours->h / 4;
	
	// Remplit la grille de cases vides
	for (x = 0; x < grille->xc; x ++)
		for (y = 0; y < grille->yc; y ++)
			grille->c [x][y] = VIDE;
	
	// Grille positionnée au milieu de l'écran
	grille->tc = grille->imgCase->w;
	grille->x1 = (ecran->w - grille->tc * grille->xc) / 2;
	grille->y1 = (ecran->h - grille->tc * grille->yc) / 2;
	grille->x2 = ecran->w - grille->x1;
	grille->y2 = ecran->h - grille->y1;
	
	// Copie les cases de la grille dans le back buffer
	dest.w = dest.h = grille->tc;
	
	for (dest.x = grille->x1; dest.x < grille->x2; dest.x += dest.w)
		for (dest.y = grille->y1; dest.y < grille->y2; dest.y += dest.h)
			SDL_BlitSurface (grille->imgCase, NULL, back, &dest);

	// Met à jour l'écran entier
	SDL_BlitSurface (back, NULL, ecran, NULL);
	SDL_UpdateRect (ecran, 0, 0, ecran->w, ecran->h);
}


// ****************************************************************************
// ****************************** Destructeur *********************************
// ****************************************************************************
void Destructeur (JOUEUR j[], GRILLE *grille)
{
	// Enlève les images de la mémoire
	SDL_FreeSurface (grille->imgCase);
	SDL_FreeSurface (grille->imgFond);  
	SDL_FreeSurface (j[0].imgFeu);
	SDL_FreeSurface (j[1].imgFeu);
	SDL_FreeSurface (j[0].imgCours);
	SDL_FreeSurface (j[1].imgCours);
	SDL_FreeSurface (j[0].imgLance);
	SDL_FreeSurface (j[1].imgLance);
	SDL_FreeSurface (j[0].imgGagne);
	SDL_FreeSurface (j[1].imgGagne);
	SDL_FreeSurface (j[0].imgPerdu);
	SDL_FreeSurface (j[1].imgPerdu);
	SDL_FreeSurface (j[0].imgAttend);
	SDL_FreeSurface (j[1].imgAttend);
	SDL_FreeSurface (j[0].imgAttendFeu);
	SDL_FreeSurface (j[1].imgAttendFeu);
}


// ****************************************************************************
// ****************************** Tester_Perdu ********************************
// ****************************************************************************
BOOL Tester_Perdu (JOUEUR j, GRILLE grille)
{
	// Teste si les 8 cases entourants le joueur existent et si elles sont vides
	if (j.x-1 >= 0 && j.y-1 >= 0        && grille.c[j.x-1][j.y-1] == VIDE) return NON;
	if (j.x-1 >= 0                      && grille.c[j.x-1][j.y]   == VIDE) return NON;
	if (j.x-1 >= 0 && j.y+1 < grille.yc && grille.c[j.x-1][j.y+1] == VIDE) return NON;
	
	if (j.y-1 >= 0        && grille.c[j.x][j.y-1] == VIDE) return NON;
	if (j.y+1 < grille.yc && grille.c[j.x][j.y+1] == VIDE) return NON;
	
	if (j.x+1 < grille.xc && j.y-1 >= 0        && grille.c[j.x+1][j.y-1] == VIDE) return NON;
	if (j.x+1 < grille.xc                      && grille.c[j.x+1][j.y]   == VIDE) return NON;
	if (j.x+1 < grille.xc && j.y+1 < grille.yc && grille.c[j.x+1][j.y+1] == VIDE) return NON;
	
	return OUI;
}


// ****************************************************************************
// ************************** Animer_Perdant_Gagnant **************************
// ****************************************************************************
void Animer_Perdant_Gagnant (JOUEUR *j1, JOUEUR *j2, JEU *jeu, CHXMENU *chxmenu, JOUEUR j[], GRILLE *grille)
{
	SDL_Rect prec1, prec2;		// Précédente position des joueurs
	SDL_Event evt;				// Pour détecter un appui sur [ESC]
	BOOL bis = OUI;				// Répète la même image d'animation
	int xSouris, ySouris;		// Coords de la souris
	const int xmin = 50;
	const int xmax = jeu->ecran->w - (j2->imgPerdu->w / j2->nPerdu) - 50;
	const int ymin = jeu->titre.y + jeu->titre.h;
	const int ymax = jeu->rMenu[0].y - j2->imgPerdu->h;
	int duree = 0;
	int vitesse = 1;
	int xdir = 0, ydir = 0;
	*chxmenu = RIEN;
	int i;
	
	// Réajuste le rectangles du joueur qui se transforme en ange
	j2->rect.w = j2->imgPerdu->w / j2->nPerdu;
	j2->rect.h = j2->imgPerdu->h;
	j2->rect.x -= 32;
	j2->rect.y -= 16;
	// Règle les animations
	j1->anim = GAGNE;
	j2->anim = PERDU;
	j1->frame = 0;
	j2->frame = 10;
	// Affiche qui a gagné
	j1->winCount++;
	if (j1->n == J2) 
	{
		Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J2GAGNE) * jeu->police1.rect.w) / 2, jeu->titre.y, 0, TXT_J2GAGNE, jeu->police1, jeu->ecran, jeu->back);
		if (j1->type == HUMAIN)
		{
			victoryChannel = Mix_PlayChannel(-1, victory, 0);
		} else {
			lossChannel = Mix_PlayChannel(-1, loss, 0);
		}
	}
	else
	{
		Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J1GAGNE) * jeu->police1.rect.w) / 2, jeu->titre.y, 0, TXT_J1GAGNE, jeu->police1, jeu->ecran, jeu->back);
		if (j1->type == HUMAIN)
		{
			victoryChannel = Mix_PlayChannel(-1, victory, 0);
		} else {
			lossChannel = Mix_PlayChannel(-1, loss, 0);
		}
	}

	for (i = 0; i < 2; i ++)
	{
		SDL_BlitSurface (grille->imgFond, &jeu->Scores[i], jeu->back, &jeu->Scores[i]);
	}

	sprintf (score, "%d", j[0].winCount);
	Afficher_Chaine (jeu->Scores[0].x, jeu->Scores[0].y, 0, score, jeu->J1ROUGE ? jeu->police3big : jeu->police2big, jeu->back, jeu->back);
	sprintf (score, "%d", j[1].winCount);
	Afficher_Chaine (jeu->Scores[1].x, jeu->Scores[1].y, 0, score, jeu->J1ROUGE ? jeu->police2big : jeu->police3big, jeu->back, jeu->back);

	SDL_BlitSurface (jeu->back, NULL, jeu->ecran, NULL);
	SDL_UpdateRects (jeu->ecran, 2, jeu->Scores);


	// Animation des joueurs
	while (1)
	{
		if (SDL_PollEvent (&evt))		// Attend un événement
		{	// Teste si le curseur passe sur l'un des boutons du menu, ou si on clique dessus, ou si on veut quitter
			*chxmenu = Tester_Menu (&evt, OUI, OUI, OUI, NON, OUI, jeu);
			if (*chxmenu) return;
		}
		else
		{	// Fait déplacer la souris s'il n'y a pas eu d'événements
			SDL_GetMouseState (&xSouris, &ySouris);
			evt.type = SDL_MOUSEMOTION;
			evt.motion.x = xSouris;
			evt.motion.y = ySouris;
			SDL_PeepEvents (&evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
		}
		
		// Impulsion du saut périlleux
		prec1 = j1->rect;
		if (j1->frame > 3  && j1->frame <= 11) j1->rect.y -= 6;
		if (j1->frame > 11 && j1->frame <= 19)  j1->rect.y += 6;
		
		// Impulsion des ailes
		if (j2->frame == 10)
			vitesse = 3;
		if (j2->frame == 16)
			vitesse = 1;
		
		// Direction de l'ange fixé pendant durée
		if (--duree < 0)
		{	duree = Alea (25, 40);
			xdir = Alea (-1, 2);
			ydir = Alea (-1, 2);
			if (xdir != 1  && j2->rect.x < xmin) xdir = 1;
			if (xdir != -1 && j2->rect.x > xmax) xdir = -1;
			if (ydir != 1  && j2->rect.y < ymin) ydir = 1;
			if (ydir != -1 && j2->rect.y > ymax) ydir = -1;
		}
		// Stop son mvt s'il s'approche d'un bord de l'écran
		if (xdir != 1  && j2->rect.x < xmin) duree = 0;
		if (xdir != -1 && j2->rect.x > xmax) duree = 0;
		if (ydir != 1  && j2->rect.y < ymin) duree = 0;
		if (ydir != -1 && j2->rect.y > ymax) duree = 0;
		prec2 = j2->rect;
		j2->rect.x += vitesse * xdir;
		j2->rect.y += vitesse * ydir;
		
		// Efface les anciennes positions des joueurs
		SDL_BlitSurface (jeu->back, &prec1, jeu->ecran, &prec1);
		SDL_BlitSurface (jeu->back, &prec2, jeu->ecran, &prec2);
		SDL_BlitSurface (jeu->back, &j1->rect, jeu->ecran, &j1->rect);
		SDL_BlitSurface (jeu->back, &j2->rect, jeu->ecran, &j2->rect);
		
		// Affiche les joueurs animés
		Afficher_Joueur (j1, jeu->ecran);
		Afficher_Joueur (j2, jeu->ecran);
		
		// Attend d'être synchronisé sur la vitesse d'affichage et rafraîchit l'écran
		Attendre_FPS ();
		SDL_UpdateRects (jeu->ecran, 1, &j1->rect);
		SDL_UpdateRects (jeu->ecran, 1, &j2->rect);
		SDL_UpdateRects (jeu->ecran, 1, &prec1);
		SDL_UpdateRects (jeu->ecran, 1, &prec2);
		
		// Passe à l'image suivante dans l'animation et reboucle si besoin
		if (bis = !bis)
		{	j1->frame ++;
			j2->frame ++;
		}
		if (j1->frame >= j1->nGagne) j1->frame = 0;
		if (j2->frame >= j2->nPerdu) j2->frame = 0;
	}
}


// ****************************************************************************
// *********************************** Jeu ************************************
// ****************************************************************************
BOOL Jeu (JEU *jeu)
{
	SDL_Event evt;			        // Sert à recueillir des infos sur les événements
	int x, y;				        // Coordonnées des clics de souris
	ACTION action;					// Action du jeu en cours
	GRILLE grille = {jeu->XGRILLE, jeu->YGRILLE};	// Grille du jeu (initialise dimensions)
	JOUEUR j[2] = {{J1, jeu->J1TYPE, jeu->J1NIVO}, {J2, jeu->J2TYPE, jeu->J2NIVO}};	// Joueurs
	BOOL bis;						// Si l'option double est activée: compteur d'action double
	BOOL repimg;					// Répète la même image d'animation
	CHXMENU chxmenu;				// Valeur retournée après un test du menu
	int n;
	int i;
	
	// Si le joueur est humain met son IA au niveau 3
	if (j[0].type == HUMAIN) j[0].nivo = 3;
	if (j[1].type == HUMAIN) j[1].nivo = 3;

	// Set both players scores to 0 to start
	for ( i = 0; i < 2; i++ )
		j[i].winCount = 0;
	
	// Rectangles des boutons du menu
	jeu->rMenu[0].y = jeu->rMenu[1].y = jeu->rMenu[2].y = jeu->rMenu[3].y = jeu->rMenu[4].y = jeu->ecran->h - jeu->police3.rect.h - 5;
	jeu->rMenu[0].h = jeu->rMenu[1].h = jeu->rMenu[2].h = jeu->rMenu[3].h = jeu->rMenu[4].h = jeu->police3.rect.h;
	jeu->rMenu[0].w = jeu->police3.rect.w * strlen (TXT_QUITTER);
	jeu->rMenu[1].w	= jeu->police3.rect.w * strlen (TXT_MENU);
	jeu->rMenu[2].w	= jeu->police3.rect.w * strlen (TXT_REFAIRE);
	jeu->rMenu[3].w = jeu->police3.rect.w * strlen (TXT_CONSEIL);
	jeu->rMenu[4].w = jeu->police3.rect.w * strlen (TXT_MUSIC);
	jeu->rMenu[0].x	= (jeu->ecran->w - (strlen (TXT_QUITTER) + strlen (TXT_MENU) + strlen (TXT_REFAIRE) + strlen (TXT_CONSEIL) + strlen (TXT_MUSIC) + 4) * jeu->police3.rect.w) / 2;
	jeu->rMenu[1].x	= jeu->rMenu[0].x + jeu->rMenu[0].w + jeu->police3.rect.w;
	jeu->rMenu[2].x	= jeu->rMenu[1].x + jeu->rMenu[1].w + jeu->police3.rect.w;
	jeu->rMenu[3].x	= jeu->rMenu[2].x + jeu->rMenu[2].w + jeu->police3.rect.w;
	jeu->rMenu[4].x	= jeu->rMenu[3].x + jeu->rMenu[3].w + jeu->police3.rect.w;
	
debut_jeu:
	Constructeur (j, &grille, jeu->J1ROUGE, jeu->ecran, jeu->back);	// Charge les images, la grille, ...

	if ((strlen(songpath) >= strlen(SNG_INTRO)) && !strncasecmp(songpath+strlen(songpath)-strlen(SNG_INTRO), SNG_INTRO, strlen(SNG_INTRO)))
	{
		if ( songCount > 0 )
		{
			stopMusic();
			strcpy( songpath, "random" );
			startMusic( songpath );
		}
	}

	// Valeurs de départ
	action = J1_DEPLACE;
	bis = OUI;
	repimg = OUI;
	chxmenu = RIEN;
	
	// Rectangle d'affichage de l'action en cours
	jeu->titre.w = strlen (TXT_J1PLACE) * jeu->police2.rect.w;
	jeu->titre.h = jeu->police2.rect.h;
	jeu->titre.x = (jeu->ecran->w - jeu->titre.w) / 2;
	jeu->titre.y = (grille.y1 - jeu->titre.h) / 2;
	
	//Rectangles for player scores
	jeu->Scores[0].w = jeu->Scores[1].w = jeu->police1.rect.w * TXT_BIG;
	jeu->Scores[0].h = jeu->Scores[1].h = jeu->police1.rect.h * TXT_BIG;
	jeu->Scores[0].y = jeu->Scores[1].y = ( jeu->ecran->h - jeu->Scores[0].h ) / 2;
	jeu->Scores[0].x = ( grille.x1 - jeu->Scores[0].w ) / 2;
	jeu->Scores[1].x = ( ( jeu->ecran->w - grille.x2 - jeu->Scores[1].w ) / 2 ) + grille.x2;
	
	//post scores
	sprintf (score, "%d", j[0].winCount);
	Afficher_Chaine (jeu->Scores[0].x, jeu->Scores[0].y, 0, score, jeu->J1ROUGE ? jeu->police3big : jeu->police2big, jeu->ecran, jeu->back);
	sprintf (score, "%d", j[1].winCount);
	Afficher_Chaine (jeu->Scores[1].x, jeu->Scores[1].y, 0, score, jeu->J1ROUGE ? jeu->police2big : jeu->police3big, jeu->ecran, jeu->back);

	// Affiche le menu en bas de l'écran
	SDL_SetAlpha (jeu->police3.img, SDL_SRCALPHA, 128);
	Afficher_Chaine (jeu->rMenu[0].x, jeu->rMenu[0].y, 0, TXT_QUITTER, jeu->police3, jeu->ecran, jeu->back);
	Afficher_Chaine (jeu->rMenu[1].x, jeu->rMenu[1].y, 0, TXT_MENU,    jeu->police3, jeu->ecran, jeu->back);
	Afficher_Chaine (jeu->rMenu[2].x, jeu->rMenu[2].y, 0, TXT_REFAIRE, jeu->police3, jeu->ecran, jeu->back);
	Afficher_Chaine (jeu->rMenu[3].x, jeu->rMenu[3].y, 0, TXT_CONSEIL, jeu->police3, jeu->ecran, jeu->back);
	Afficher_Chaine (jeu->rMenu[4].x, jeu->rMenu[4].y, 0, TXT_MUSIC,   jeu->police3, jeu->ecran, jeu->back);
	SDL_SetAlpha (jeu->police3.img, SDL_SRCALPHA, 255);

	Placer_Joueurs (jeu, j, &grille, &chxmenu);	// Pour choisir la position de départ
	if (chxmenu) goto chx_menu;

	// Boucle de jeu principale
	while (1)
	{
		// Teste si l'un des joueurs a perdu
		if (((action == J1_DEPLACE || action == J2_TROU) && Tester_Perdu (j[0], grille)) || ((action == J2_DEPLACE || action == J1_TROU) && Tester_Perdu (j[1], grille)))
		{	
			// Efface l'affichage de l'action en cours
			SDL_BlitSurface (jeu->back, &jeu->titre, jeu->ecran, &jeu->titre);
			SDL_UpdateRects (jeu->ecran, 1, &jeu->titre);
			
			// Affiche les joueurs sur l'écran
			SDL_BlitSurface (jeu->back, &j[0].rect, jeu->ecran, &j[0].rect);
			SDL_BlitSurface (jeu->back, &j[1].rect, jeu->ecran, &j[1].rect);
			Afficher_Joueur (j[0].y > j[1].y ? &j[1] : &j[0], jeu->ecran);
			Afficher_Joueur (j[0].y > j[1].y ? &j[0] : &j[1], jeu->ecran);
			SDL_UpdateRects (jeu->ecran, 1, &j[0].rect);
			SDL_UpdateRects (jeu->ecran, 1, &j[1].rect);
			
			// Quel joueur a perdu ?
			if ((action == J1_DEPLACE || action == J2_TROU) && Tester_Perdu (j[0], grille)) n = 0;
			else n = 1;
			
			// Le perdant se suicide
			grille.c[j[n].x][j[n].y] = VIDE;
			Placer_Trou (j[n].x, j[n].y, j, n, &grille, jeu, NON, &chxmenu);
			if (chxmenu) goto chx_menu;

			// Anime les joueurs (le gagnant danse et le perdant s'envole en ange)
			Animer_Perdant_Gagnant (&j[!n], &j[n], jeu, &chxmenu, j, &grille);
			if (chxmenu) goto chx_menu;
		}
		
		// Si c'est à un humain de jouer, anime le joueur en attendant un événement
		while ((action / 2 == 0 && j[0].type == HUMAIN) || (action / 2 == 1 && j[1].type == HUMAIN))
		{	
			// Affiche l'action en cours
			SDL_BlitSurface (jeu->back, &jeu->titre, jeu->ecran, &jeu->titre);
			switch (action)
			{
				case J1_DEPLACE:	Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J1DEPLACE) * jeu->police2.rect.w) / 2, jeu->titre.y, 0, TXT_J1DEPLACE, jeu->J1ROUGE ? jeu->police3 : jeu->police2, jeu->ecran, jeu->ecran);
									break;
				case J1_TROU:		Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J1TROU)    * jeu->police2.rect.w) / 2, jeu->titre.y, 0, TXT_J1TROU,    jeu->J1ROUGE ? jeu->police3 : jeu->police2, jeu->ecran, jeu->ecran);
									break;
				case J2_DEPLACE:	Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J2DEPLACE) * jeu->police2.rect.w) / 2, jeu->titre.y, 0, TXT_J2DEPLACE, jeu->J1ROUGE ? jeu->police2 : jeu->police3, jeu->ecran, jeu->ecran);
									break;
				case J2_TROU:		Afficher_Chaine ((jeu->ecran->w - strlen (TXT_J2TROU)    * jeu->police2.rect.w) / 2, jeu->titre.y, 0, TXT_J2TROU,    jeu->J1ROUGE ? jeu->police2 : jeu->police3, jeu->ecran, jeu->ecran);
									break;
			}
			SDL_UpdateRects (jeu->ecran, 1, &jeu->titre);
			
			// Affiche les joueurs sur l'écran
			SDL_BlitSurface (jeu->back, &j[0].rect, jeu->ecran, &j[0].rect);
			SDL_BlitSurface (jeu->back, &j[1].rect, jeu->ecran, &j[1].rect);
			Afficher_Joueur (j[0].y > j[1].y ? &j[1] : &j[0], jeu->ecran);
			Afficher_Joueur (j[0].y > j[1].y ? &j[0] : &j[1], jeu->ecran);
			SDL_UpdateRects (jeu->ecran, 1, &j[0].rect);
			SDL_UpdateRects (jeu->ecran, 1, &j[1].rect);
			Attendre_FPS ();
			
			if (SDL_PollEvent (&evt))		// Attend un événement
			{
				if (evt.type == SDL_KEYUP)
				{
					handleKey (evt.key);
				}
				if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1)		// Clic bouton gauche
				{
					// Récupère les coordonnées de la case cliquée, -1 si en dehors de la grille
					x = Clic2CaseX (evt.button.x, grille);
					y = Clic2CaseY (evt.button.y, grille);
					
					// Arrête l'attente d'événement si le clic est sur une case de la grille
					if (x != -1 && y != -1) break;
				}
				
				// Teste si le curseur passe sur l'un des boutons du menu, ou si on clique dessus, ou si on veut quitter
				chxmenu = Tester_Menu (&evt, OUI, OUI, OUI, OUI, OUI, jeu);
				if (chxmenu == CONSEIL)
				{	
					if (action == J1_DEPLACE || action == J1_TROU) n = 0;
					else n = 1;
					
					if (action == J1_DEPLACE || action == J2_DEPLACE)
					{	Ordi_Deplace (&x, &y, j, n, grille);
						Deplacer_Joueur (x, y, j, n, &grille, jeu, OUI, &chxmenu);
					}
					else
					{	Ordi_Trou (&x, &y, j, !n, grille);
						Placer_Trou (x, y, j, n, &grille, jeu, OUI, &chxmenu);
					}
					
					if (chxmenu) goto chx_menu;
				}
				else if (chxmenu != RIEN) goto chx_menu;
			}
			else
			{	// Fait déplacer la souris s'il n'y a pas eu d'événements
				SDL_GetMouseState (&x, &y);
				evt.type = SDL_MOUSEMOTION;
				evt.motion.x = x;
				evt.motion.y = y;
				SDL_PeepEvents (&evt, 1, SDL_ADDEVENT, SDL_MOUSEMOTIONMASK);
			}
			
			// Règle les animations pour le joueur qui attend de jouer et celui qui ne bouge pas
			switch (action)
			{
				case J1_DEPLACE:	j[0].anim = ATTEND;
									if (repimg = !repimg) j[0].frame ++;
									if (j[0].frame >= j[0].nAttend) j[0].frame = 0;
									j[1].anim = COURSB;
									j[1].frame = 0;
									break;
				
				case J1_TROU:		j[0].anim = ATTENDFEU;
									if (repimg = !repimg) j[0].frame ++;
									if (j[0].frame >= j[0].nAttendFeu) j[0].frame = 0;
									j[1].anim = COURSB;
									j[1].frame = 0;
									break;
				
				case J2_DEPLACE:	j[1].anim = ATTEND;
									if (repimg = !repimg) j[1].frame ++;
									if (j[1].frame >= j[1].nAttend) j[1].frame = 0;
									j[0].anim = COURSB;
									j[0].frame = 0;
									break;
				
				case J2_TROU:		j[1].anim = ATTENDFEU;
									if (repimg = !repimg) j[1].frame ++;
									if (j[1].frame >= j[1].nAttendFeu) j[1].frame = 0;
									j[0].anim = COURSB;
									j[0].frame = 0;
									break;
			}
		}
		
		// Fait jouer les joueurs humains ou ordis
		switch (action)
		{
			case J1_DEPLACE:
				if (j[0].type == ORDI) Ordi_Deplace (&x, &y, j, 0, grille);
				
				if (Deplacer_Joueur (x, y, j, 0, &grille, jeu, NON, &chxmenu))
				{	
					if (chxmenu) goto chx_menu;
					if (jeu->DOUBLE_DEPLACE && !(bis = !bis)) break;
					action = J1_TROU;
				}
				break;
			
			case J1_TROU:
				if (j[0].type == ORDI) Ordi_Trou (&x, &y, j, 1, grille);
				
				if (Placer_Trou (x, y, j, 0, &grille, jeu, NON, &chxmenu))
				{	
					if (chxmenu) goto chx_menu;
					if (jeu->DOUBLE_BOULEFEU && !(bis = !bis)) break;
					action = J2_DEPLACE;
				}
				break;
			
			case J2_DEPLACE:
				if (j[1].type == ORDI) Ordi_Deplace (&x, &y, j, 1, grille);
				
				if (Deplacer_Joueur (x, y, j, 1, &grille, jeu, NON, &chxmenu))
				{	
					if (chxmenu) goto chx_menu;
					if (jeu->DOUBLE_DEPLACE && !(bis = !bis)) break;
					action = J2_TROU;
				}
				break;
			
			case J2_TROU:
				if (j[1].type == ORDI) Ordi_Trou (&x, &y, j, 0, grille);
				
				if (Placer_Trou (x, y, j, 1, &grille, jeu, NON, &chxmenu))
				{	
					if (chxmenu) goto chx_menu;
					if (jeu->DOUBLE_BOULEFEU && !(bis = !bis)) break;
					action = J1_DEPLACE;
					
					// Place aléatoirement une boule si l'option est activée
					if (jeu->INTEMP) while (!(Intemperie (j, &grille, jeu, &chxmenu)));
					if (chxmenu) goto chx_menu;
				}
				break;
		}
	}

chx_menu:
	if (chxmenu == QUITTER)
	{	Destructeur (j, &grille);
		return OUI;
	}
	if (chxmenu == MENU)
	{	Destructeur (j, &grille);
		return NON;
	}
	if (chxmenu == REFAIRE)
	{	Destructeur (j, &grille);
		goto debut_jeu;
	}
}
