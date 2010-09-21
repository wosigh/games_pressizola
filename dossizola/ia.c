#include <SDL/SDL.h>
#include "isola.h"

// ****************************************************************************
// ****************************** Calcul_Choix ********************************
// ****************************************************************************
void Calcul_Choix (GRILLE grille, int Copie_Grille[][YCASEMAX+2], int Passages, int *Compteur)
{
  int x,y;
  int i,j,Temp;
  int Temp_Grille[XCASEMAX+2][YCASEMAX+2];
  
  //Calcul pour chaque case le nombre de cases vides adjacentes
  for (x = 1; x <= grille.xc; x ++)
    for (y = 1; y <= grille.yc; y ++)
      {
	if (Copie_Grille[x][y] != 0)      
	  {                                 
	    Temp = 0;
	    for (i = x-1; i <= x+1; i ++)
	      for (j = y-1; j <= y+1; j ++)
		Temp = Temp + Copie_Grille[i][j]; 
	    Temp_Grille[x][y] = Temp;    
	  }
	else Temp_Grille[x][y] = 0;
      }

  for (y = 1; y <= grille.yc; y ++)
    for (x = 1; x <= grille.xc; x ++)
      Copie_Grille[x][y] = Temp_Grille[x][y];

  //On prévoit pour plusieurs tours
  *Compteur = *Compteur + 1;
  //On voit quelle case libre adjacente à le plus de cases libres adjacentes ...
  if (*Compteur != Passages ) Calcul_Choix(grille, Copie_Grille, Passages, Compteur);  
}

// ****************************************************************************
// ***************************** Meilleur_Choix *******************************
// ****************************************************************************
void Meilleur_Choix (GRILLE grille, int Copie_Grille[][YCASEMAX+2], int Passages, JOUEUR j[], int n,MAX JFoncJ[],MAX PasJFoncJ [])
{
  int u, v, Compteur = 0, Taille = 1;
  //Coordonnées dans la nouvelle grille
  int newx = j[n].x + 1, newy = j[n].y + 1, newx2 = j[!n].x + 1, newy2 = j[!n].y + 1; 
  MAX Max;

  //Initialise la grille
  for (u = 0; u <= grille.xc+1; u ++)
    for (v = 0; v <= grille.yc+1; v ++)
      Copie_Grille[u][v] = 0;

  //Traduit la grille de jeu pour les calculs
  for (u = 0; u < grille.xc; u ++)
    for (v = 0; v < grille.yc; v ++)
	if ((grille.c[u][v] == VIDE) || (grille.c[u][v] == !n)) 
	  Copie_Grille[u+1][v+1] = 1;

  Calcul_Choix(grille, Copie_Grille, Passages, &Compteur);
  
  //Enregistre toutes les cases adjacentes
  for (u = newx-1; u <= newx+1; u ++)
    for (v = newy-1; v <= newy+1; v ++)
      if ((Copie_Grille[u][v] != 0) && !(u == newx && v == newy))
	{
	  Max.x = u;
	  Max.y = v;
	  Max.nb = Copie_Grille[u][v];
	  JFoncJ[Taille] = Max;
	  Taille ++;
	}

  Max.nb = Taille-1;
  Max.x = 0;
  Max.y = 0;
  JFoncJ[0] = Max;
  
  //Enregistre toutes les cases adjacentes à l'adversaire
  Taille = 0; 
  for (u = newx2-1; u <= newx2+1; u ++)
    for (v = newy2-1; v <= newy2+1; v ++)
      if ((Copie_Grille[u][v] != 0) && !(u == newx2 && v == newy2))
	{
	  Max.x = u;
	  Max.y = v;
	  Max.nb = Copie_Grille[u][v];
	  PasJFoncJ[Taille] = Max;
	  Taille ++;
	}
  Max.nb = Taille-1;
  Max.x = 0;
  Max.y = 0;
  PasJFoncJ[0] = Max;

}

// ****************************************************************************
// ******************************* Ordi_Place *********************************
// ****************************************************************************
void Ordi_Place (int *x, int *y, JOUEUR j, GRILLE grille)
{
  int u, v, Compteur = 0;
  int Copie_Grille[XCASEMAX+2][YCASEMAX+2]; //Copie agrandie du jeu en cours
  int Passage = 2;                          //Nombre de passages dans la fonction Calcul_Choix
  MAX Max, Choix[MAXCASES];                 //Mémorise les cases de plus grandes valeurs avec leurs coordonnées

  //Initialise la grille
  for (u = 0; u <= grille.xc+1; u ++)
    for (v = 0; v <= grille.yc+1; v ++)
      Copie_Grille[u][v] = 0;
  
  //Traduit la grille de jeu pour les calculs
  for (u = 0; u < grille.xc; u ++)
    for (v = 0; v < grille.yc; v ++)
      if ((grille.c[u][v] == VIDE) || (grille.c[u][v] == j.n)) 
	  Copie_Grille[u+1][v+1] = 1;

  Calcul_Choix(grille, Copie_Grille, Passage, &Compteur);

  //Cherche la valeur maximale
  Max.nb = 1;
  for (u = 0; u <= grille.xc+1; u ++)
    for (v = 0; v <= grille.yc+1; v ++)
      if (Copie_Grille[u][v] > Max.nb)
	Max.nb = Copie_Grille[u][v];
  //Enregistre les coordonnées de toutes les cases égales à ce maximum
  Compteur = 0;
  for (u = 0; u <= grille.xc+1; u ++)
    for (v = 0; v <= grille.yc+1; v ++)
      if (Copie_Grille[u][v] == Max.nb)  
	{
	  Max.x = u;
	  Max.y = v;
	  Choix[Compteur] = Max;
	  Compteur ++;
	}

  //Choisi aléatoirement l'une de ces cases
  u = Alea (0, Compteur-1);
  Max = Choix[u];
  *x = Max.x-1;
  *y = Max.y-1;
 
}


// ****************************************************************************
// ****************************** Ordi_Deplace ********************************
// ****************************************************************************
void Ordi_Deplace (int *x, int *y, JOUEUR j[], int n, GRILLE grille)
{
  
  MAX JFoncJ[MAXCASES], JFoncPasJ[MAXCASES], PasJFoncJ[MAXCASES], PasJFoncPasJ[MAXCASES];
  //Correspond aux données autour de j et !j en fonction d'eux-mêmes ou de leur adversaire
  int Copie_Grille[XCASEMAX+2][YCASEMAX+2], Copie2_Grille[XCASEMAX+2][YCASEMAX+2];  
  //Copie agrandie du jeu en cours
  int u, v, Compteur = 0, Compteur2 = 0, Passage;
  int newx = j[n].x + 1, newy = j[n].y + 1, newx2 = j[!n].x + 1, newy2 = j[!n].y + 1; 
  //Coordonnées dans la nouvelle grille
  int max[MAXCASES];
  MAX Max, Choix[MAXCASES];

  if (j[n].nivo == 1)
    {

      Passage = 1;

      Meilleur_Choix (grille, Copie_Grille, Passage, j, n, JFoncJ, PasJFoncJ);

      //Calcul s'il y a une valeur sous la maximale
      Compteur = 1;
      max[0]=0;
      for (u = 1; u <= 9; u ++)
	for (v = 1; v <= JFoncJ[0].nb; v ++)
	  if ((JFoncJ[v].nb == u) && (u != max[Compteur-1]))
	    {
	      max[Compteur] = u;
	      Compteur ++;
	    }

      if (Compteur != 2) Max.nb =  max[Compteur-2];
      else Max.nb =  max[Compteur-1];

      //Enregistre toutes les cases dépassant cette valeur
      Compteur = 0;
      for (u = 1; u <= JFoncJ[0].nb; u ++)
	if (JFoncJ[u].nb >= Max.nb) 
	  {
	  Choix[Compteur] = JFoncJ[u];
	  Compteur ++;
	  }

      //Choisi aléatoirement l'une de ces cases
      u = Alea (0, Compteur-1);
      Max = Choix[u];

      *x = Max.x-1;
      *y = Max.y-1;

    }

  if (j[n].nivo == 2)
    {

      Passage = 3;
      
      Meilleur_Choix (grille, Copie_Grille, Passage, j, n, JFoncJ, PasJFoncJ);
      
      //Calcule la valeur maximale
      Max.nb = JFoncJ[1].nb;
      for (u = 2; u <= JFoncJ[0].nb; u++)
	if ((JFoncJ[u].nb > Max.nb) && !(u == newx && v == newy))
	  Max.nb = JFoncJ[u].nb;
      
      //Enregistre toutes les cases de cette valeur
      for (u = 1; u <= JFoncJ[0].nb; u ++)
	if (JFoncJ[u].nb == Max.nb) 
	  {
	    Choix[Compteur] = JFoncJ[u];
	    Compteur ++;
	  }
      
      //Choisi aléatoirement l'une de ces cases
      u = Alea (0, Compteur-1);
      Max = Choix[u];
      
      *x = Max.x-1;
      *y = Max.y-1;

    }

  if (j[n].nivo == 3)
    {

      Passage = 3;
      
      Meilleur_Choix (grille, Copie2_Grille, Passage, j, !n, JFoncPasJ, PasJFoncPasJ);
      u = Copie2_Grille[newx2][newy2];
      
      Meilleur_Choix (grille, Copie_Grille, Passage, j, n, JFoncJ, PasJFoncJ);
      v = Copie_Grille[newx][newy];
      
      if (u > v)
	{
	  //Calcule la valeur maximale
	  Max.nb = JFoncJ[1].nb;
	  for (u = 2; u <= JFoncJ[0].nb; u++)
	    if ((JFoncJ[u].nb > Max.nb) && !(u == newx && v == newy))
	      Max.nb = JFoncJ[u].nb;
	}
      
      else
	{
	  
	  //Calcule la valeur maximale
	  Max.nb = JFoncJ[1].nb;
	  for (u = 2; u <= JFoncJ[0].nb; u++)
	    if ((JFoncJ[u].nb > Max.nb) && !(u == newx && v == newy))
	      Max.nb = JFoncJ[u].nb;
	  max[Compteur]=Max.nb;
	  
	  //Calcul s'il y a une valeur sous sous la maximale
	  for (u = 1; u <= JFoncJ[0].nb; u++)
	    if ((JFoncJ[u].nb < Max.nb) && !(u == newx && v == newy))
	      Max.nb = JFoncJ[u].nb;
	  
	  if (Max.nb != max[Compteur])
	    {
	      
	      Max.nb = 0;
	      for (u = 1; u <= JFoncJ[0].nb; u++)
		if ((JFoncJ[u].nb > Max.nb) && (JFoncJ[u].nb != max[Compteur]) && !(u == newx && v == newy))  Max.nb = JFoncJ[u].nb;
	      max[Compteur+1] = Max.nb;
	      
	      for (u = 1; u <= JFoncJ[0].nb; u++)
		if ((JFoncJ[u].nb < max[Compteur+1]) && !(u == newx && v == newy))
		  Max.nb = JFoncJ[u].nb;
	      
	      if (Max.nb != max[Compteur+1])
		{
		  Max.nb = 0;
		  for (u = 1; u <= JFoncJ[0].nb; u++)
		    if ((JFoncJ[u].nb > Max.nb) && (JFoncJ[u].nb != max[Compteur+1]) && !(u == newx && v == newy))  Max.nb = JFoncJ[u].nb;
		}
	      
	      else Max.nb = max[Compteur+1];
	      
	    }
	  
	  else Max.nb =  max[Compteur];
	  
	}
      
      //Enregistre toutes les cases dépassant cette valeur
      Compteur = 0;
      for (u = 1; u <= JFoncJ[0].nb; u ++)
	if (JFoncJ[u].nb >= Max.nb) 
	  {
	    Choix[Compteur] = JFoncJ[u];
	    Compteur ++;
	  }
      
      //Donne aux cases la valeur du point de vue de l'adversaire
      for (u = 0; u < Compteur; u ++)
	Choix[u].nb = Copie2_Grille[Choix[u].x][Choix[u].y];

      //Calcule la valeur maximale
      Max.nb = Choix[0].nb;
      for (u = 1; u < Compteur; u++)
	if (Choix[u].nb > Max.nb)
	  Max.nb = Choix[u].nb;

      //Enregistre toutes les cases de cette valeur
      for (u = 0; u < Compteur; u ++)
	if (Choix[u].nb == Max.nb) 
	  {
	    Choix[Compteur2] = Choix[u];
	    Compteur2 ++;
	  }
      
      //Choisi aléatoirement l'une de ces cases
      u = Alea (0, Compteur2-1);
      Max = Choix[u];
      
      *x = Max.x-1; 
      *y = Max.y-1;       
    }

}


// ****************************************************************************
// ******************************* Ordi_Trou **********************************
// ****************************************************************************
void Ordi_Trou (int *x, int *y, JOUEUR j[], int n, GRILLE grille)
{ 
  MAX JFoncJ[MAXCASES], JFoncPasJ[MAXCASES], PasJFoncJ[MAXCASES], PasJFoncPasJ[MAXCASES];
  //Correspond aux données autour de j et !j en fonction d'eux-mêmes ou de leur adversaire
  int Copie_Grille[XCASEMAX+2][YCASEMAX+2], Copie2_Grille[XCASEMAX+2][YCASEMAX+2];  
  //Copie agrandie du jeu en cours
  int u, v, Compteur = 0, Compteur2 = 0, Passage;
  int newx = j[n].x + 1, newy = j[n].y + 1, newx2 = j[!n].x + 1, newy2 = j[!n].y + 1; 
  //Coordonnées dans la nouvelle grille
  int max[MAXCASES];
  MAX Max, Choix[MAXCASES];


  if (j[n].nivo == 1)
    {

      Passage = 1;

      Meilleur_Choix (grille, Copie_Grille, Passage, j, n, JFoncJ, PasJFoncJ);

      //Calcul s'il y a une valeur sous la maximale
      Compteur = 1;
      max[0]=0;
      for (u = 1; u <= 9; u ++)
	for (v = 1; v <= JFoncJ[0].nb; v ++)
	  if ((JFoncJ[v].nb == u) && (u != max[Compteur-1]))
	    {
	      max[Compteur] = u;
	      Compteur ++;
	    }

      if (Compteur != 2) Max.nb =  max[Compteur-2];
      else Max.nb =  max[Compteur-1];

      //Enregistre toutes les cases dépassant cette valeur
      Compteur = 0;
      for (u = 1; u <= JFoncJ[0].nb; u ++)
	if (JFoncJ[u].nb >= Max.nb) 
	  {
	  Choix[Compteur] = JFoncJ[u];
	  Compteur ++;
	  }
      
      //Choisi aléatoirement l'une de ces cases
      u = Alea (0, Compteur-1);
      Max = Choix[u];
      
      *x = Max.x-1;
      *y = Max.y-1;
     
    }

if (j[n].nivo == 2)
    {

      Passage = 3;

      Meilleur_Choix (grille, Copie_Grille, Passage, j, n, PasJFoncPasJ, JFoncPasJ);

      //Calcule la valeur maximale
      Max.nb =PasJFoncPasJ[1].nb;
      for (u = 2; u <= PasJFoncPasJ[0].nb; u++)
	if ((PasJFoncPasJ[u].nb > Max.nb) && !(u == newx && v == newy))
	  Max.nb = PasJFoncPasJ[u].nb;

      //Enregistre toutes les cases de cette valeur
      for (u = 1; u <=PasJFoncPasJ[0].nb; u ++)
	if (PasJFoncPasJ[u].nb == Max.nb) 
	  {
	    Choix[Compteur] =PasJFoncPasJ[u];
	    Compteur ++;
	  }

      //Choisi aléatoirement l'une de ces cases
      u = Alea (0, Compteur-1);
      Max = Choix[u];

      *x = Max.x-1;
      *y = Max.y-1;

    }

 if (j[n].nivo == 3)
   {
    
     Passage = 3;
     
     
     Meilleur_Choix (grille, Copie2_Grille, Passage, j, !n, JFoncPasJ, PasJFoncPasJ);
     u = Copie2_Grille[newx2][newy2];
     
     Meilleur_Choix (grille, Copie_Grille, Passage, j, n, JFoncJ, PasJFoncJ);
     v = Copie_Grille[newx][newy];
     
     
     if (u <= v)
       {
	 //Calcule la valeur maximale
	 Max.nb =JFoncJ[1].nb;
	 for (u = 2; u <= JFoncJ[0].nb; u++)
	   if ((JFoncJ[u].nb > Max.nb) && !(u == newx && v == newy))
	     Max.nb = JFoncJ[u].nb;
	 
	 //Enregistre toutes les cases de cette valeur
	 for (u = 1; u <=JFoncJ[0].nb; u ++)
	   if (JFoncJ[u].nb == Max.nb) 
	     {
	       Choix[Compteur] =JFoncJ[u];
	       Compteur ++;
	     }
	 
	 //Choisi aléatoirement l'une de ces cases
	 u = Alea (0, Compteur-1);
	 Max = Choix[u];
	 
	 *x = Max.x-1;
	 *y = Max.y-1;
	 
       }
     
     else 
       {
	 //Calcule la valeur maximale
	 Max.nb = JFoncJ[1].nb;
	 for (u = 2; u <= JFoncJ[0].nb; u++)
	   if ((JFoncJ[u].nb > Max.nb) && !(u == newx && v == newy))
	     Max.nb = JFoncJ[u].nb;
	 max[Compteur]=Max.nb;
	 
	 //Calcul s'il y a une valeur sous la maximale
	 for (u = 1; u <= JFoncJ[0].nb; u++)
	   if ((JFoncJ[u].nb < Max.nb) && !(u == newx && v == newy))
	     Max.nb = JFoncJ[u].nb;
	 
	 if (Max.nb != max[Compteur])
	   {
	     Max.nb = 0;
	     for (u = 1; u <= JFoncJ[0].nb; u++)
	       if ((JFoncJ[u].nb > Max.nb) && (JFoncJ[u].nb != max[Compteur]) && !(u == newx && v == newy))
		 Max.nb = JFoncJ[u].nb;
	   }
	 
	 else Max.nb =  max[Compteur]; 
	 
	 //Enregistre toutes les cases dépassant cette valeur
	 Compteur = 0;
	 for (u = 1; u <= JFoncJ[0].nb; u ++)
	   if (JFoncJ[u].nb >= Max.nb) 
	     {
	       Choix[Compteur] = JFoncJ[u];
	       Compteur ++;
	     }
	 
	 //Donne aux cases la valeur du point de vue de l'adversaire
	 for (u = 0; u < Compteur; u ++)
	   Choix[u].nb = Copie2_Grille[Choix[u].x][Choix[u].y];
	 
	 //Calcule la valeur minimale
	 Max.nb = Choix[0].nb;
	 for (u = 1; u < Compteur; u++)
	   if (Choix[u].nb < Max.nb)
	     Max.nb = Choix[u].nb;
	 
	 //Enregistre toutes les cases de cette valeur
	 for (u = 0; u < Compteur; u ++)
	   if (Choix[u].nb == Max.nb) 
	     {
	       Choix[Compteur2] = Choix[u];
	       Compteur2 ++;
	     }
	 
	 //Choisi aléatoirement l'une de ces cases
	 u = Alea (0, Compteur2-1);
	 Max = Choix[u];
	 
	 *x = Max.x-1;
	 *y = Max.y-1; 
	 
	 
       } 
     
   }
 
}
