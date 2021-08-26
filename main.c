#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "instance.h"
#include "parseur.h"
#include "heuristique.h"
#include "solution.h"



int main()
{
  //tableau stockant les noms des strategies de tri
  char* tab[4]={"Aléatoire","Valeurs","Ratio Valeurs / Poids","Ratio Valeurs / PoidsDeLaDimensionCritique"};

  //on parse le fichier
  char* filename="mknapcb/_mknapcb1_res.txt";
  instanceDB * row = parser(filename);

  //on ouvre le fichier de résultat
  FILE * sortie = fopen("sortie.txt", "w");
  fprintf(sortie, "Résultat du fichier %s \n\n",filename);
  float secondes = CLOCKS_PER_SEC;

  //on parcourt les instances
  for(int i = 0; i < row->nbInstances;i++){
    fprintf(sortie, "Instance n°%d :\n",i+1);

    //on parcourt toutes les méthodes heuristiques
    for(int j = 0; j<4;j++){
      fprintf(sortie, "\tMéthode n°%d : %s\n",j+1,tab[j]);

      //on initialies une solution
      solution * sol = solution_create_and_init(row->listeInstances[i]);

      //on enregistre l'heure avant d'appliquer la méthode heuristique
      float tpsDebut=clock();

      //on applique la méthode heuristique
      sol->tabSol = heuristique(row->listeInstances[i], j);
      sol->evalDeSol= objectif(sol,row->listeInstances[i]);

      //on écrit la valeur finale obtenue dans le fichier
      fprintf(sortie, "\tValeur finale : %d\n",sol->evalDeSol);
      
      //on enregistre l'heure après avoir appliquer la méthode heuristique
      float tpsFin=clock();
      //on calcule la durée totale d'exécution de la méthode
      float tpsTotal=((float)tpsFin-(float)tpsDebut)/(float)secondes;
      //on note le temps de calcul de cette méthode heuristique
      fprintf(sortie,"\tTemps de calcul : %f secondes\n\n",tpsTotal);

      //on détruit la solution pour libérer la mémoire
      solution_destroy(sol);
    }
  }
  return 0;
}
