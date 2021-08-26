#include "instance.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//crée et initialise une structure instance
instance * instance_create_and_init(void){
  //on alloue la mémoire pour le tableau d'instances
  instance * row =  malloc(sizeof(instance));
  //on initialise les valeurs
  row->nbObjets = 0;
  row->nbDimensions = 0;
  row->poidsMax = NULL;
  row->listeObjets = NULL;
  return row;
}

//détruit une structure instance
void instance_destroy(instance * row) {
  //on libère les mémoires allouées
  for(int i = 0; i < row->nbObjets; i++){
    free(row->listeObjets[i].listePoids);
  }
  free(row->listeObjets);
  free(row->poidsMax);
  free(row);
}

//détruit une structure instanceDB
void instanceDB_destroy(instanceDB * row){
  //on libère les mémoires allouées
  for(int i = 0; i < row->nbInstances; i++){
    instance_destroy(row->listeInstances[i]);
  }
  free(row->listeInstances);
  free(row);
}


//Lit le fichier passé en paramètre et initialise une instance
instance * readInstance(FILE * fichier){
  //on crée une structure pour stocker l'instance
  instance * row = instance_create_and_init();
  int ignored; //valeur pour ignorer certaines valeurs du fichier
  
  //
  float clock_tick = CLOCKS_PER_SEC;
  clock_t tpsDebut,tpsFin; //horloge de début et de fin
  //on récupère l'heure avant de commencer à lire l'instance
  tpsDebut=clock();
  
  //on recupère le nombre d'objets et le nombre de dimensiosn
  fscanf(fichier,"%d", &row->nbObjets);
  fscanf(fichier,"%d", &row->nbDimensions);
  
  //on ignore les deux valeurs suivantes
  fscanf(fichier,"%d", &ignored);
  fscanf(fichier,"%d", &ignored);

  //on alloue la mémoire pour stocker les objets
  row->listeObjets = malloc(row->nbObjets * sizeof(objet));
  
  //on ignore les valeurs 0 et 1 (solution particulière)
  for(int i = 0; i < row->nbObjets; i++){
     fscanf(fichier,"%d", &ignored);
  }

  //on récupère les valeurs des objets
  for(int j = 0; j < row->nbObjets; j++){
     fscanf(fichier,"%d", &row->listeObjets[j].valeur);
  }

  // on alloue la mémoire pour les poids des différents objets
  for(int i = 0; i < row->nbDimensions; i++){
    for(int j = 0; j < row->nbObjets; j++){
      row->listeObjets[j].listePoids = malloc(row->nbDimensions * sizeof(int));
    }
  }

  //on recupere les poids des différents objets selon la dimension
  for(int i = 0; i < row->nbDimensions; i++){
    for(int j = 0; j < row->nbObjets; j++){
      fscanf(fichier,"%d", &row->listeObjets[j].listePoids[i]);
    }
  }

  //on alloue la memoire pour stocker les poids maximum de chaque dimension
  row->poidsMax = malloc(row->nbDimensions * sizeof(int));
  //on recupère les poids maximum de chaque dimension
  for(int i = 0; i < row->nbDimensions; i++){
    int tmp;
    fscanf(fichier,"%d", &tmp);
    row->poidsMax[i]=tmp;
  }

  //on récupère l'heure après avoir lu l'instance
  tpsFin=clock();

  //on calcule la durée de lecture et on convertit en secondes
  float tpsTotal = (float) ((tpsFin - tpsDebut) / (float)clock_tick);
  return row;
}

//Affiche une instance avec tous ses objets
void afficher_instance(instance * instance){
  printf("\nInstance : \n");
  for(int i = 0; i < instance->nbObjets; i++){
    printf("\nObjet n°%d \n",i);
    afficherObjet(&instance->listeObjets[i], instance->nbDimensions);
    printf("\n");
  }
}

//Affiche un objet, son numéro, sa valeur et tous ses poids
void afficherObjet(objet * obj, int dim){
  printf("Valeur  = %d \n", obj->valeur);
  for(int i = 0; i < dim; i++){
    printf(" %d  ", obj->listePoids[i]);
  }
}

