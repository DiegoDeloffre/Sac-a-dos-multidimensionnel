#include <stdio.h>
#include <stdlib.h>
#include "solution.h"

//crée et initialise une structure solution
solution * solution_create_and_init(instance* inst){
  //on alloue la mémoire
  solution* sol = (solution *)malloc(sizeof(solution));
  //on initialise les valeurs
  sol->evalDeSol = 0;
  sol->tabSol = NULL;
  return sol;
}

//détruit une stucture solution
void solution_destroy(solution * sol){
  //on libère les mémoires allouées
  free(sol->tabSol);
  free(sol);
}

//initialise une solution particulière de l'instance passée en paramètre
solution * initSol(instance * row){
  //
  FILE* fichier = NULL;
  solution * sol = solution_create_and_init(row);

  //on ouvre le fichier pour lire l'instance
  fichier = fopen("mknapcb/_mknapcb1_res.txt","r");
  if(fichier==NULL){
      perror("erreur d'ouverture");
      exit(EXIT_FAILURE);
  }
  int ignored;

  //on ignore les premières valeurs
  fscanf(fichier,"%d", &ignored);
  fscanf(fichier,"%d", &ignored);
  fscanf(fichier,"%d", &ignored);
  fscanf(fichier,"%d", &ignored);
  fscanf(fichier,"%d", &ignored);
  
  //on récupère la solution particulière (0 et 1) et on la stocke
  //dans le tableau de la solution
  for(int i = 0; i < row->nbObjets; i++){
     fscanf(fichier,"%d", &sol->tabSol[i]);
  }
  fclose(fichier);
  return sol;
}

//vérifie qu'une solution sol est correcte, c'est à dire qu'aucun des poids totaux
//des objets ne dépasse les poids maximaux indiqués pour chaque dimension
//renvoie 1 si la solution est correcte et 0 sinon
int isSolutionRight(solution * sol, instance * row){
  //on alloue un tableau pour stocker les sommes des poids des objets selon la dimension
  int * poidsMaxSol = malloc(sizeof(int)*row->nbDimensions);

  //on parcourt tout le tableau de la solution
  for(int i = 0; i< row->nbObjets; i++){
    //si il y a un 1 = objet choisi
    if(sol->tabSol[i]==1){
      // on parcourt toutes les dimensions
      for(int j =0; j< row->nbDimensions;j++){
        //on additionne les poids des objets
        poidsMaxSol[j]+=row->listeObjets[i].listePoids[j];
        //si un poids additionné dépasse
        if(poidsMaxSol[j] > row->poidsMax[j]){
          free(poidsMaxSol);
          return 0;//return false
        }
      }
    }
  }
  free(poidsMaxSol);
  return 1;//return true
}

//affiche la solution avec les objets sélectionnés
void afficherSolution(solution *sol, instance * row){
  printf("Les objets selectionnes sont les suivants : \n");
  // on parcourt tous les objets de l'instance
  for(int i = 0; i < row->nbObjets; i++){
    //si sol =1 cad si on prend l'objet dans le sac
    if(sol->tabSol[i] == 1){
      //on affiche l'objet
      printf("Objet n°%d : \n",i);
      afficherObjet(&row->listeObjets[i], row->nbDimensions);
    }
  }
}

//écrit la solution avec les objets sélectionnés dans un fichier
void writeSolutionFile(solution *sol, FILE * fichier, instance * row){
  fprintf(fichier,"Les objets selectionnes sont les suivants :\n");
  // on parcourt tous les objets de l'instance
  for(int i =0; i < row->nbObjets ; i++){
    //si sol =1 cad si on prend l'objet dans le sac
    if (sol->tabSol[i]==1){
      //on affiche l'objet dans le fichier
      fprintf(fichier," n°%d ",i);
    }
  }
  fprintf(fichier,"\n");
  //on affiche la valeur totale des objets
  fprintf(fichier,"Valeur totale des objets : %d\n",sol->evalDeSol);

}

//évalue la performance de la solution
int objectif(solution * sol, instance * row){
  int total;
  //on parcourt tous les objets
  for(int i =0; i < row->nbObjets ; i++){
    //si on prend l'objet
    if (sol->tabSol[i]==1){
      //on ajoute la valeur de l'objet pris
      total += row->listeObjets[i].valeur;
    }
  }
  return total;
}
