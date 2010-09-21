#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "isola.h"

// ****************************************************************************
// ********************************** Aide ************************************
// ****************************************************************************
void Aide (JEU *jeu, SDL_Surface *imgMenu)
{
	SDL_Event evt;
	int centre = jeu->ecran->w / 2;		// Position centr� au milieu de l'�cran
	
	SDL_BlitSurface (imgMenu, NULL, jeu->ecran, NULL);
	SDL_BlitSurface (imgMenu, NULL, jeu->back,  NULL);
	SDL_UpdateRect (jeu->ecran, 0, 0, jeu->ecran->w, jeu->ecran->h);
	
	if (Afficher_Chaine (centre - strlen (TXT_AIDE1) * jeu->police1.rect.w / 2, 10, 0, TXT_AIDE1, jeu->police3, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE2) * jeu->police1.rect.w / 2, 50, 0, TXT_AIDE2, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE3) * jeu->police1.rect.w / 2, 80, 0, TXT_AIDE3, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE4) * jeu->police1.rect.w / 2, 110, 0, TXT_AIDE4, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE5) * jeu->police1.rect.w / 2, 140, 0, TXT_AIDE5, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE6) * jeu->police1.rect.w / 2, 200, 0, TXT_AIDE6, jeu->police3, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE7) * jeu->police1.rect.w / 2, 240, 0, TXT_AIDE7, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE8) * jeu->police1.rect.w / 2, 270, 0, TXT_AIDE8, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE9) * jeu->police1.rect.w / 2, 300, 0, TXT_AIDE9, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE10) * jeu->police1.rect.w / 2, 330, 0, TXT_AIDE10, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE11) * jeu->police1.rect.w / 2, 390, 0, TXT_AIDE11, jeu->police3, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE12) * jeu->police1.rect.w / 2, 420, 0, TXT_AIDE12, jeu->police3, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE13) * jeu->police1.rect.w / 2, 460, 0, TXT_AIDE13, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE14) * jeu->police1.rect.w / 2, 490, 0, TXT_AIDE14, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE15) * jeu->police1.rect.w / 2, 520, 0, TXT_AIDE15, jeu->police2, jeu->ecran, jeu->back)) return;
	if (Afficher_Chaine (centre - strlen (TXT_AIDE16) * jeu->police1.rect.w / 2, 550, 0, TXT_AIDE16, jeu->police2, jeu->ecran, jeu->back)) return;
	
	while (1)
	if (SDL_PollEvent (&evt))
	{	
		// Quitte sur un appui de la touche [ESC]
		if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) return;
		
		// Clic de souris avec le bouton gauche
		if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1) return;
	}
}


// ****************************************************************************
// ******************************* Afficher_IA ********************************
// ****************************************************************************
void Afficher_IA (int nivo, SDL_Rect src[], SDL_Rect dest[], SDL_Surface *imgNivo, SDL_Surface *ecran, SDL_Surface *back, SDL_Surface *imgMenu)
{
	int i;
	
	// R�affiche les images repr�sentant les 3 niveaux d'intelligence artificielle
	for (i = 0; i < 3; i ++)
	{
		// R�affiche l'image de fond dans le back buffer
		SDL_BlitSurface (imgMenu, &dest[i], back, &dest[i]);
		
		// L'image n'est pas transparente si c'est celle du niveau s�l�ctionn� (0) sinon elle l'est (150)
		SDL_SetAlpha (imgNivo, SDL_SRCALPHA, (nivo == i+1) ? 0 : 150);
		
		// Copie l'image dans le back buffer
		SDL_BlitSurface (imgNivo, &src[i], back, &dest[i]);
		
		// Copie le back buffer sur l'�cran et met � jour
		SDL_BlitSurface (back, &dest[i], ecran, &dest[i]);
		SDL_UpdateRects (ecran, 1, &dest[i]);
	}
}


// ****************************************************************************
// ********************************** Menu ************************************
// ****************************************************************************
BOOL Menu (JEU *jeu)
{
	SDL_Surface *imgDossi1, *imgDossi2, *imgNivo;	// Surface des images
	SDL_Surface *imgMenu;					// Surface du fond d'�cran
	SDL_Rect src, dossi1, dossi2;			// Rects des dessins des joueurs
	SDL_Rect type1, type2;					// Rects du type des joueurs
	SDL_Rect nivo1[3], nivo2[3], nivo[3];	// Rects du niveau de l'IA
	SDL_Rect op1, op2, op3, op4;			// Rects des options
	SDL_Rect jouer, quitter, aide;			// Rects de boutons
	SDL_Rect xgrille, ygrille;				// Rects des dimensions de la grille
	SDL_Event evt;							// Sert � r�cup�rer les �v�nements du clavier et de la souris
	int centre1 = jeu->ecran->w / 4 - 20;	// Position centr� sur le quart gauche de l'�cran
	int centre2 = jeu->ecran->w - centre1;	// Position centr� sur le quart droit  de l'�cran
	int frame = 0;							// Image en cours dans l'animation
	int nbImgDossi = 20;					// Nb d'images dans l'animation des Do'SSis
	char ch[100];							// Chaine de caract�re temporaire
	BOOL bis = OUI;							// R�p�te la m�me image d'animation
	int i;
	
	// Charge les images
	sprintf (fichier, "%s%s", imgpath, IMG_MENU);
	if (!(imgMenu   = IMG_Load (fichier)))	  ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J1PERDU);
	if (!(imgDossi1 = IMG_Load (fichier))) ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_J2PERDU);
	if (!(imgDossi2 = IMG_Load (fichier))) ImageErreur (fichier);
	sprintf (fichier, "%s%s", imgpath, IMG_NIVO);
	if (!(imgNivo   = IMG_Load (fichier)))	  ImageErreur (fichier);
	
	// D�finit la couleur transparente des images
	SDL_SetColorKey (imgDossi1, SDL_SRCCOLORKEY, SDL_MapRGB (imgDossi1->format, 255, 0, 255));
	SDL_SetColorKey (imgDossi2, SDL_SRCCOLORKEY, SDL_MapRGB (imgDossi2->format, 255, 0, 255));
	SDL_SetColorKey (imgNivo,   SDL_SRCCOLORKEY, SDL_MapRGB (imgNivo->format,   255, 0, 255));
	
debut:
	// Affiche l'image de fond
	SDL_BlitSurface (imgMenu, NULL, jeu->ecran, NULL);
	SDL_BlitSurface (imgMenu, NULL, jeu->back,  NULL);
	SDL_UpdateRect (jeu->ecran, 0, 0, jeu->ecran->w, jeu->ecran->h);
	
	// Rectangle source de l'animation des dossis, destinations sur l'�cran
	src.x = src.y = 0;
	src.w = imgDossi1->w / nbImgDossi;
	src.h = imgDossi1->h;
	dossi1.w = dossi2.w = src.w;
	dossi1.h = dossi2.h = src.h;
	dossi1.y = dossi2.y = 315 + TXT_DECALY;
	dossi1.x = centre1 - dossi1.w / 2;
	dossi2.x = centre2 - dossi2.w / 2;
	
	// Rectangles de choix du type de joueurs (humain ou ordi)
	if (jeu->J1TYPE == HUMAIN) type1.w = strlen (TXT_HUMAIN) * jeu->police1.rect.w;
	else type1.w = strlen (TXT_ORDI) * jeu->police1.rect.w;
	if (jeu->J2TYPE == HUMAIN) type2.w = strlen (TXT_HUMAIN) * jeu->police1.rect.w;
	else type2.w = strlen (TXT_ORDI) * jeu->police1.rect.w;
	type1.h = type2.h = jeu->police1.rect.h;
	type1.y = type2.y = 415 + TXT_DECALY;
	type1.x = centre1 - type1.w / 2;
	type2.x = centre2 - type2.w / 2;
	
	// Rectangles des choix des niveaux de l'IA
	for (i = 0; i < 3; i ++)
	{
		nivo[i].w = nivo1[i].w = nivo2[i].w = imgNivo->w / 3;
		nivo[i].h = nivo1[i].h = nivo2[i].h = imgNivo->h;
		nivo[i].y = 0;
		nivo[i].x = i * imgNivo->w / 3;
		nivo1[i].y = nivo2[i].y = 450 + TXT_DECALY;
		nivo1[i].x = centre1 + (i*2 - 3) * imgNivo->w / 6;
		nivo2[i].x = centre2 + (i*2 - 3) * imgNivo->w / 6;
	}
	
	// Rectangles des choix des options
	if (jeu->INTEMP) op1.w = strlen (TXT_OUI) * jeu->police1.rect.w;
	else op1.w = strlen (TXT_NON) * jeu->police1.rect.w;
	if (jeu->DOUBLE_DEPLACE) op2.w = strlen (TXT_DOUBLE) * jeu->police1.rect.w;
	else op2.w = strlen (TXT_SIMPLE) * jeu->police1.rect.w;
	if (jeu->DOUBLE_BOULEFEU) op3.w = strlen (TXT_DOUBLE) * jeu->police1.rect.w;
	else op3.w = strlen (TXT_SIMPLE) * jeu->police1.rect.w;
	op4.w = strlen ("XXX FPS")	* jeu->police1.rect.w;
	op1.h = op2.h = op3.h = op4.h = jeu->police1.rect.h;
	op1.x = op2.x = op3.x = op4.x = 600 + TXT_DECALX;
	op1.y = 540 + TXT_DECALY;
	op2.y = 580 + TXT_DECALY;
	op3.y = 620 + TXT_DECALY;
	op4.y = 660 + TXT_DECALY;
	
	// Rectangle du bouton de d�marrage du jeu
	jouer.w = strlen (TXT_JOUER) * jeu->police1.rect.w;
	jouer.h = jeu->police1.rect.h;
	jouer.x = (jeu->ecran->w - jouer.w) / 2;
	jouer.y = 350 + TXT_DECALY;
	
	// Rectangle du bouton de l'aide
	aide.w = strlen (TXT_AIDE) * jeu->police1.rect.w;
	aide.h = jeu->police1.rect.h;
	aide.x = (jeu->ecran->w - aide.w) / 2;
	aide.y = 390 + TXT_DECALY;
	
	// Rectangle du bouton quitter
	quitter.w = strlen (TXT_QUITTER) * jeu->police1.rect.w;
	quitter.h = jeu->police1.rect.h;
	quitter.x = (jeu->ecran->w - quitter.w) / 2;
	quitter.y = 430 + TXT_DECALY;
	
	// Rectangles des choix de dimensions de la grille
	xgrille.w = ygrille.w = jeu->police1.rect.w;
	xgrille.h = ygrille.h = jeu->police1.rect.h;
	xgrille.y = ygrille.y = 700 + TXT_DECALY;
	xgrille.x = 600 + TXT_DECALX;
	ygrille.x = 700 + TXT_DECALX;
	
	// Affiche les textes sur l'�cran
	Afficher_Chaine (centre1 - (strlen (TXT_J1NOM) * jeu->police1.rect.w) / 2, 280 + TXT_DECALY, 0, TXT_J1NOM, jeu->police1, jeu->ecran, jeu->back);
	Afficher_Chaine (centre2 - (strlen (TXT_J2NOM) * jeu->police1.rect.w) / 2, 280 + TXT_DECALY, 0, TXT_J2NOM, jeu->police1, jeu->ecran, jeu->back);
	
	if (jeu->J1TYPE == HUMAIN)
		Afficher_Chaine (type1.x, type1.y, 0, TXT_HUMAIN, jeu->police2, jeu->ecran, jeu->back);
	else
	{	Afficher_Chaine (type1.x, type1.y, 0, TXT_ORDI,	jeu->police2, jeu->ecran, jeu->back);
		Afficher_IA (jeu->J1NIVO, nivo, nivo1, imgNivo, jeu->ecran, jeu->back, imgMenu);
	}
	
	if (jeu->J2TYPE == HUMAIN)
		Afficher_Chaine (type2.x, type2.y, 0, TXT_HUMAIN, jeu->police2, jeu->ecran, jeu->back);
	else
	{	Afficher_Chaine (type2.x, type2.y, 0, TXT_ORDI,	jeu->police2, jeu->ecran, jeu->back);
		Afficher_IA (jeu->J2NIVO, nivo, nivo2, imgNivo, jeu->ecran, jeu->back, imgMenu);
	}
	
	Afficher_Chaine (590 + TXT_DECALX - strlen (TXT_INTEMP)		 * jeu->police1.rect.w, 540 + TXT_DECALY, 0, TXT_INTEMP,	jeu->police1, jeu->ecran, jeu->back);
	Afficher_Chaine (590 + TXT_DECALX - strlen (TXT_DEPLACE)	 * jeu->police1.rect.w, 580 + TXT_DECALY, 0, TXT_DEPLACE,	jeu->police1, jeu->ecran, jeu->back);
	Afficher_Chaine (590 + TXT_DECALX - strlen (TXT_BOULEFEU)	 * jeu->police1.rect.w, 620 + TXT_DECALY, 0, TXT_BOULEFEU,	jeu->police1, jeu->ecran, jeu->back);
	Afficher_Chaine (590 + TXT_DECALX - strlen (TXT_VITESSEJEU)	 * jeu->police1.rect.w, 660 + TXT_DECALY, 0, TXT_VITESSEJEU,jeu->police1, jeu->ecran, jeu->back);
	Afficher_Chaine (680 + TXT_DECALX - strlen (TXT_TAILLEGRILLE) * jeu->police1.rect.w, 700 + TXT_DECALY, 0, TXT_TAILLEGRILLE,jeu->police1, jeu->ecran, jeu->back);
	
	if (jeu->INTEMP)
		Afficher_Chaine (op1.x, op1.y, 0, TXT_OUI, jeu->police2, jeu->ecran, jeu->back);
	else
		Afficher_Chaine (op1.x, op1.y, 0, TXT_NON, jeu->police2, jeu->ecran, jeu->back);
	
	if (jeu->DOUBLE_DEPLACE)
		Afficher_Chaine (op2.x, op2.y, 0, TXT_DOUBLE, jeu->police2, jeu->ecran, jeu->back);
	else
		Afficher_Chaine (op2.x, op2.y, 0, TXT_SIMPLE, jeu->police2, jeu->ecran, jeu->back);
	
	if (jeu->DOUBLE_BOULEFEU)
		Afficher_Chaine (op3.x, op3.y, 0, TXT_DOUBLE, jeu->police2, jeu->ecran, jeu->back);
	else
		Afficher_Chaine (op3.x, op3.y, 0, TXT_SIMPLE, jeu->police2, jeu->ecran, jeu->back);
	
	sprintf (ch, "%d FPS", FPS);
	Afficher_Chaine (op4.x, op4.y, 0, ch, jeu->police2, jeu->ecran, jeu->back);
	
	sprintf (ch, "%d", jeu->XGRILLE);
	Afficher_Chaine (xgrille.x, xgrille.y, 0, ch, jeu->police2, jeu->ecran, jeu->back);
	sprintf (ch, "%d", jeu->YGRILLE);
	Afficher_Chaine (ygrille.x, ygrille.y, 0, ch, jeu->police2, jeu->ecran, jeu->back);
	
	Afficher_Chaine (jouer.x, jouer.y, 0, TXT_JOUER, jeu->police3, jeu->ecran, jeu->back);
	Afficher_Chaine (aide.x, aide.y, 0, TXT_AIDE, jeu->police3, jeu->ecran, jeu->back);
	Afficher_Chaine (quitter.x, quitter.y, 0, TXT_QUITTER, jeu->police3, jeu->ecran, jeu->back);
	
	// Boucle d'attente des �v�nements
	while (1)
	{
		// Anime les dossis avec un effet de motiooon bluuuurrrr grace � l'alpha bleeending
		if (bis = !bis) frame ++;
		if (frame >= nbImgDossi) frame = 0;
		SDL_BlitSurface (imgMenu, &dossi1, jeu->back, &dossi1);
		SDL_BlitSurface (imgMenu, &dossi2, jeu->back, &dossi2);
		
		SDL_SetAlpha (imgDossi1, SDL_SRCALPHA, 200);
		SDL_SetAlpha (imgDossi2, SDL_SRCALPHA, 200);
		src.x = frame * src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi2 : imgDossi1, &src, jeu->back, &dossi1);
		src.x = ((frame+nbImgDossi/2) % nbImgDossi)* src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi1 : imgDossi2, &src, jeu->back, &dossi2);
		
		SDL_SetAlpha (imgDossi1, SDL_SRCALPHA, 175);
		SDL_SetAlpha (imgDossi2, SDL_SRCALPHA, 175);
		src.x = ((frame+1) % nbImgDossi) * src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi2 : imgDossi1, &src, jeu->back, &dossi1);
		src.x = ((frame+1+nbImgDossi/2) % nbImgDossi) * src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi1 : imgDossi2, &src, jeu->back, &dossi2);
		
		SDL_SetAlpha (imgDossi1, SDL_SRCALPHA, 150);
		SDL_SetAlpha (imgDossi2, SDL_SRCALPHA, 150);
		src.x = ((frame+2) % nbImgDossi) * src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi2 : imgDossi1, &src, jeu->back, &dossi1);
		src.x = ((frame+2+nbImgDossi/2) % nbImgDossi) * src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi1 : imgDossi2, &src, jeu->back, &dossi2);
		
		SDL_SetAlpha (imgDossi1, SDL_SRCALPHA, 0);
		SDL_SetAlpha (imgDossi2, SDL_SRCALPHA, 0);
		src.x = ((frame+3) % nbImgDossi) * src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi2 : imgDossi1, &src, jeu->back, &dossi1);
		src.x = ((frame+3+nbImgDossi/2) % nbImgDossi) * src.w;
		SDL_BlitSurface (jeu->J1ROUGE ? imgDossi1 : imgDossi2, &src, jeu->back, &dossi2);
		
		SDL_BlitSurface (jeu->back, &dossi1, jeu->ecran, &dossi1);
		SDL_BlitSurface (jeu->back, &dossi2, jeu->ecran, &dossi2);
		SDL_UpdateRects (jeu->ecran, 1, &dossi1);
		SDL_UpdateRects (jeu->ecran, 1, &dossi2);
		Attendre_FPS ();
		
		// Traite l'�v�nement suivant dans la file d'attente s'il y en a
		if (SDL_PollEvent (&evt))
		{	
			// Quitte si l'on ferme la fenetre, sur un appui de la touche [ESC], un clic sur quitter
			if (evt.type == SDL_QUIT ||(evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE) || (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1 && Dans_Rect (evt.button.x, evt.button.y, quitter)))
			{
				if (Questionner (TXT_QSTQUITTER, jeu->police1, jeu->police2, jeu->police3, jeu->ecran))
				{	
					SDL_FreeSurface (imgMenu);
					SDL_FreeSurface (imgDossi1);
					SDL_FreeSurface (imgDossi2);
					SDL_FreeSurface (imgNivo);
					return OUI;
				}
			}
			
			// Clic de souris avec le bouton gauche
			if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1)				
			{
				// Si on clique sur le bouton jouer, �� commence le jeu
				if (Dans_Rect (evt.button.x, evt.button.y, jouer))
				{	
					// Message d'attente
					Informer (TXT_PATIENTEZ, jeu->police1, jeu->ecran);
					SDL_FreeSurface (imgMenu);
					SDL_FreeSurface (imgDossi1);
					SDL_FreeSurface (imgDossi2);
					SDL_FreeSurface (imgNivo);
					return NON;
				}
				
				// Si on clique sur le bouton aide, �� lance l'aide
				if (Dans_Rect (evt.button.x, evt.button.y, aide))
				{
					Aide (jeu, imgMenu);
					goto debut;
				}
				
				// Si on clique sur l'un des deux dossis, ils s'inversent
				if (Dans_Rect (evt.button.x, evt.button.y, dossi1) || Dans_Rect (evt.button.x, evt.button.y, dossi2)) jeu->J1ROUGE = !jeu->J1ROUGE;
				
				// Si on clique sur la taille de la grille
				if (Dans_Rect (evt.button.x, evt.button.y, xgrille))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &xgrille, jeu->ecran, &xgrille);
					SDL_BlitSurface (imgMenu, &xgrille, jeu->back,  &xgrille);
					SDL_UpdateRects (jeu->ecran, 1, &xgrille);
					
					// Change la taille et affiche le nouveau chiffre
					if (++ jeu->XGRILLE > XCASEMAX) jeu->XGRILLE = XCASEMIN;
					sprintf (ch, "%d", jeu->XGRILLE);
					Afficher_Chaine (xgrille.x, xgrille.y, CARACT_VITESSE, ch, jeu->police2, jeu->ecran, jeu->back);
				}
				if (Dans_Rect (evt.button.x, evt.button.y, ygrille))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &ygrille, jeu->ecran, &ygrille);
					SDL_BlitSurface (imgMenu, &ygrille, jeu->back,  &ygrille);
					SDL_UpdateRects (jeu->ecran, 1, &ygrille);
					
					// Change la taille et affiche le nouveau chiffre
					if (++ jeu->YGRILLE > YCASEMAX) jeu->YGRILLE = YCASEMIN;
					sprintf (ch, "%d", jeu->YGRILLE);
					Afficher_Chaine (ygrille.x, ygrille.y, CARACT_VITESSE, ch, jeu->police2, jeu->ecran, jeu->back);
				}
				
				// Active ou d�sactive la pluie al�atoire de m�t�orites
				if (Dans_Rect (evt.button.x, evt.button.y, op1))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &op1, jeu->ecran, &op1);
					SDL_BlitSurface (imgMenu, &op1, jeu->back,  &op1);
					SDL_UpdateRects (jeu->ecran, 1, &op1);
					
					// Inverse la valeur et affiche le nouveau choix
					jeu->INTEMP = !jeu->INTEMP;
					op1.w = strlen (jeu->INTEMP ? TXT_OUI : TXT_NON) * jeu->police1.rect.w;
					Afficher_Chaine (op1.x, op1.y, CARACT_VITESSE, jeu->INTEMP ? TXT_OUI : TXT_NON, jeu->police2, jeu->ecran, jeu->back);
				}
				
				// Change le d�placement (simple en double)
				if (Dans_Rect (evt.button.x, evt.button.y, op2))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &op2, jeu->ecran, &op2);
					SDL_BlitSurface (imgMenu, &op2, jeu->back,  &op2);
					SDL_UpdateRects (jeu->ecran, 1, &op2);
					
					// Inverse la valeur et affiche le nouveau choix
					jeu->DOUBLE_DEPLACE = !jeu->DOUBLE_DEPLACE;
					op2.w = strlen (jeu->DOUBLE_DEPLACE ? TXT_DOUBLE : TXT_SIMPLE) * jeu->police2.rect.w;
					Afficher_Chaine (op2.x, op2.y, CARACT_VITESSE, jeu->DOUBLE_DEPLACE ? TXT_DOUBLE : TXT_SIMPLE, jeu->police2, jeu->ecran, jeu->back);
				}
				
				// Change le nombre de trou � placer (simple ou double)
				if (Dans_Rect (evt.button.x, evt.button.y, op3))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &op3, jeu->ecran, &op3);
					SDL_BlitSurface (imgMenu, &op3, jeu->back, &op3);
					SDL_UpdateRects (jeu->ecran, 1, &op3);
					
					// Inverse la valeur et affiche le nouveau choix
					jeu->DOUBLE_BOULEFEU = !jeu->DOUBLE_BOULEFEU;
					op3.w = strlen (jeu->DOUBLE_BOULEFEU ? TXT_DOUBLE : TXT_SIMPLE) * jeu->police2.rect.w;
					Afficher_Chaine (op3.x, op3.y, CARACT_VITESSE, jeu->DOUBLE_BOULEFEU ? TXT_DOUBLE : TXT_SIMPLE, jeu->police2, jeu->ecran, jeu->back);
				}
				
				// Change le nombre d'images par seconde
				if (Dans_Rect (evt.button.x, evt.button.y, op4))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &op4, jeu->ecran, &op4);
					SDL_BlitSurface (imgMenu, &op4, jeu->back, &op4);
					SDL_UpdateRects (jeu->ecran, 1, &op4);
					
					// Inverse la valeur et affiche le nouveau choix
					FPS += 16;
					if (FPS > 128) FPS = 32;
					sprintf (ch, "%d FPS", FPS);
					Afficher_Chaine (op4.x, op4.y, CARACT_VITESSE, ch, jeu->police2, jeu->ecran, jeu->back);	
				}
				
				// Si on clique sur un des niveaux d'intelligence, il devient actif (si le joueur est ORDI)
				for (i = 0; i < 3; i ++)
				{
					if (Dans_Rect (evt.button.x, evt.button.y, nivo1[i]) && jeu->J1TYPE == ORDI)
					{	jeu->J1NIVO = i+1;
						Afficher_IA (jeu->J1NIVO, nivo, nivo1, imgNivo, jeu->ecran, jeu->back, imgMenu);
					}
					if (Dans_Rect (evt.button.x, evt.button.y, nivo2[i]) && jeu->J2TYPE == ORDI)
					{	jeu->J2NIVO = i+1;
						Afficher_IA (jeu->J2NIVO, nivo, nivo2, imgNivo, jeu->ecran, jeu->back, imgMenu);
					}
				}
				
				// Si on clique sur le type d'un joueur (HUMAIN ou ORDI), il change
				if (Dans_Rect (evt.button.x, evt.button.y, type1))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &type1, jeu->ecran, &type1);
					SDL_BlitSurface (imgMenu, &type1, jeu->back,  &type1);
					SDL_UpdateRects (jeu->ecran, 1, &type1);
					
					// Inverse le type du joueur et affiche le nouveau type
					jeu->J1TYPE = !jeu->J1TYPE;
					if (jeu->J1TYPE == ORDI)
					{	
						// Affiche que le joueur est ordi
						type1.w = strlen (TXT_ORDI) * jeu->police1.rect.w;
						type1.x = centre1 - type1.w / 2;
						Afficher_Chaine (type1.x, type1.y, CARACT_VITESSE, TXT_ORDI, jeu->police2, jeu->ecran, jeu->back);
						// Affiche le choix du niveau de l'IA
						Afficher_IA (jeu->J1NIVO, nivo, nivo1, imgNivo, jeu->ecran, jeu->back, imgMenu);
					}
					else
					{	
						// Efface le choix du niveau de l'IA
						for (i = 0; i < 3; i ++)
						{
							SDL_BlitSurface (imgMenu, &nivo1[i], jeu->back,  &nivo1[i]);
							SDL_BlitSurface (imgMenu, &nivo1[i], jeu->ecran, &nivo1[i]);
							SDL_UpdateRects (jeu->ecran, 1, &nivo1[i]);
						}
						// Affiche que le joueur est humain
						type1.w = strlen (TXT_HUMAIN) * jeu->police1.rect.w;
						type1.x = centre1 - type1.w / 2;
						Afficher_Chaine (type1.x, type1.y, CARACT_VITESSE, TXT_HUMAIN, jeu->police2, jeu->ecran, jeu->back);
					}
				}
				if (Dans_Rect (evt.button.x, evt.button.y, type2))
				{
					// Efface l'ancien texte
					SDL_BlitSurface (imgMenu, &type2, jeu->ecran, &type2);
					SDL_BlitSurface (imgMenu, &type2, jeu->back,  &type2);
					SDL_UpdateRects (jeu->ecran, 1, &type2);
					
					// Inverse le type du joueur et affiche le nouveau type
					jeu->J2TYPE = !jeu->J2TYPE;
					if (jeu->J2TYPE == ORDI)
					{	
						// Affiche que le joueur est ordi
						type2.w = strlen (TXT_ORDI) * jeu->police1.rect.w;
						type2.x = centre2 - type2.w / 2;
						Afficher_Chaine (type2.x, type2.y, CARACT_VITESSE, TXT_ORDI, jeu->police2, jeu->ecran, jeu->back);
						// Affiche le choix du niveau de l'IA
						Afficher_IA (jeu->J2NIVO, nivo, nivo2, imgNivo, jeu->ecran, jeu->back, imgMenu);
					}
					else
					{	
						// Efface le choix du niveau de l'IA
						for (i = 0; i < 3; i ++)
						{
							SDL_BlitSurface (imgMenu, &nivo2[i], jeu->back,  &nivo2[i]);
							SDL_BlitSurface (imgMenu, &nivo2[i], jeu->ecran, &nivo2[i]);
							SDL_UpdateRects (jeu->ecran, 1, &nivo2[i]);
						}
						// Affiche que le joueur est humain
						type2.w = strlen (TXT_HUMAIN) * jeu->police1.rect.w;
						type2.x = centre2 - type2.w / 2;
						Afficher_Chaine (type2.x, type2.y, CARACT_VITESSE, TXT_HUMAIN, jeu->police2, jeu->ecran, jeu->back);
					}
				}
			}
		}
	}
}