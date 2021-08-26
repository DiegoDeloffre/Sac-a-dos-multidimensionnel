#ifndef SOLUTION_H
#define SOLUTION_H
#include <stdio.h>
#include <stdlib.h>
#include "parseur.h"

typedef struct{
  int evalDeSol;
  int * tabSol;
}solution;

solution * solution_create_and_init(instance* inst);
void solution_destroy(solution * sol);

solution * initSol(instance * row);

void afficherSolution(solution* sol, instance * row);

int isSolutionRight(solution * sol, instance * row);

void writeSolutionFile(solution * sol, FILE * fichier, instance * row);

int objectif(solution * sol, instance * row); 

#endif