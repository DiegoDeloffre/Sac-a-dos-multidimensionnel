#ifndef HEURISTIQUE_H
#define HEURISTIQUE_H
#include <stdio.h>
#include <stdlib.h>
#include "solution.h"

typedef struct {
  int nbElement;
  int tailleMax;  //
  float* list;  //valeurs
  int* numero;  //numeros des Objets
}liste;

liste * liste_create_and_init(instance * row);
void list_destroy(liste * list);
void insert_liste(liste * list,float elem);


float ratioPoidsDim(instance * row, liste * list);
float ratioPoidsDimCritique(instance * row, liste * list);
void aleatoire(instance * row, liste * list);
float valeurObjets(instance * row, liste * list);

int* heuristique(instance *row, int strategie);
void tri_liste(instance * row,liste * list,int typeStrategie);

#endif
