#ifndef INSTANCE_H
#define INSTANCE_H
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int valeur;
  int * listePoids;
} objet;

typedef struct Instance{
  int nbObjets;
  int nbDimensions;
  objet * listeObjets;
  int * poidsMax;
} instance;

typedef struct{
  int nbInstances;
  instance ** listeInstances;
} instanceDB;

instance * instance_create_and_init(void);
void instance_destroy(instance * row);
void instanceDB_destroy(instanceDB * row);

instance * readInstance(FILE * fichier);

void afficher_instance(instance * row);
void afficherObjet(objet * obj, int dim);

#endif