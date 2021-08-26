#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heuristique.h"

//crée et initialise une structure liste 
liste * liste_create_and_init(instance * row){
  //on alloue la mémoire de l'objet
  liste * list = (liste *)malloc(sizeof(liste));
  //on initialise les valeurs
  list->tailleMax=row->nbObjets;
  list->nbElement =0;
  //on alloue la mémoire pour stocker les valeurs des objets
  list->list = (float *)malloc((list->tailleMax)*sizeof(float));
  //on alloue al mémoire pour conserver les numéros des objets
  list->numero = (int*)calloc((list->tailleMax),sizeof(int));
  return list;
}

//détruit une structure liste
void list_destroy(liste * list){
  //on libère les mémoires allouées
  free(list->list);
  free(list->numero);
  free(list);
}


//permet d'insérer un élément elem dans la liste liste selon un ordre décroissant
void insert_liste(liste * list,float elem){
  //si la liste n'est pas vide
  if(list->nbElement != 0){
    int num = list->nbElement;
    int j = list->nbElement - 1; //on part du dernier élément du tableau
    int end = 0;
    while (j>=0 && !end){
      //si l'élément passé en paramètre est plsu grand que l'élément courant
      if (elem > list->list[j]){
        //si on n'est pas sur le premier élément du tableau
        if(j != 0) {
          //on décale l'élément courant vers la fin du tableau
          list->list[j+1]=list->list[j];
          //on décale le numero de l'élément que l'on vient de déplacer
          list->numero[j+1]=list->numero[j];
        }else{
          //on décale l'élément courant vers la fin du tableau
          list->list[j+1]=list->list[j];
          //on décale le numero de l'élément que l'on vient de déplacer
          list->numero[j+1]=list->numero[j];

          //on place l'objet et son numéro dans les tableaux correspondants
          list->list[j] = elem;
          list->numero[j] = num;
        }
      }else{
        //l'élément passé en paramètre est plus petit que l'élément courant
        //on ajoute l'élément et son numéro à la fin de la liste dans les tableaux correspondants
        list->list[j+1]=elem;
        list->numero[j+1] = num;
        end = 1;
      }
      j--;
    }
    //on incrémente le nombre d'éléments
    list->nbElement++;
  }else{
    //sinon si la liste est vide
    //on ajoute l'element à l'indice 0 de la liste
    list->list[0]=elem;
    //on conserve le numero de l'objet à l'indice 0
    list->numero[0] = 0;
    //on incrémente le nombre d'éléments
    list->nbElement++;
  }
}

//calcule le ratio entre la valeur de l'objet et la somme de tous ses poids
float ratioPoidsDim(instance * row, liste * list){
  float poids = 0.0;  //somme des poids

  //on recupere la valeur de l'objet
  int valeur=row->listeObjets[list->nbElement].valeur;

  //on additionne les poids
  for (int i=0; i < row->nbDimensions;i++){
    poids += row->listeObjets[list->nbElement].listePoids[i];
  }
  //on renvoie le ratio entre la valeur de l'objet et la somme de ses poids
  return valeur/poids;
}

float ratioPoidsDimCritique(instance * row, liste * list){
  //on recupere la valeur de l'objet
  int valeur = row->listeObjets[list->nbElement].valeur;

  //on recupere le poids max de la dimension la plus critique
  float max = row->listeObjets[list->nbElement].listePoids[0];
  for (int i=1; i<row->nbDimensions;i++){
    if ((float)(row->listeObjets[list->nbElement].listePoids[i]) > max){
      max = row->listeObjets[list->nbElement].listePoids[i];
    }
  }
  //on renvoie le ratio entre la velur et le poids max de la dimension critique
  return valeur/max;
}

//modifie l'ordre des éléments d'une liste list passé en paramètre de façon aléatoire
void aleatoire(instance * row, liste * list){
  srand(time(NULL));
  list->nbElement=row->nbObjets;

  //on copie les éléments de l'instance dans la liste ainsi que leur numero
  for(int i=0;i<row->nbObjets;i++)
    {
      list->numero[i]=i;
      list->list[i]=row->listeObjets[i].valeur;
    }

  //on parcourt la liste 
  for(int i = row->nbObjets-1 ;i > 0 ;i--)
    {
      int tmp;
      //calcul du nouvel index de l'element
      int indice = (int) (((double)rand()* (double)i/ (double)RAND_MAX) +1);

      //on recupere l'element du tableau à l'indice indice
      tmp = list->numero[indice];

      //on echange sa place avec l'element courant
      list->numero[indice] = list->numero[i];
      list->numero[i] = tmp;
    }
}


//trie une liste list passée en paramètre selon une strategie passée en paramètre
void tri_liste(instance * row,liste * list,int strategie){
  if (strategie == 0){ //strategie aleatoire
    aleatoire(row,list);
  }
  else{
    
    float elem = 0.0;
    // on commence au début de la liste
    list->nbElement=0;
    while (list->nbElement < list->tailleMax){
      if(strategie==1){ //strategie de tri par valeur
        elem = row->listeObjets[list->nbElement].valeur;

      }else if(strategie==2){ //strategie de tri par ratio valeur / poids
        elem = ratioPoidsDim(row,list);

      }else if(strategie==3){ //strategie de tri par ration valeur / poidsDeLaDimensionCritique
        elem = ratioPoidsDimCritique(row,list);
      }
      //on insère dans la liste le nouvel élément en fonction de du choix de la strategie
      insert_liste(list, elem);
    }
  }
}

//calcule une heuristique selon une strategie passée en oaramètre
int* heuristique(instance *row, int strategie){
  //on initialise une structure liste
  liste * list;
  list = liste_create_and_init(row);

  //on initialise un tableau de 0 pour stocker la solution
  int * solution = (int *)calloc(row->nbObjets,sizeof(int));
  int j = 0; // sert à récuperer les éléments dans la liste

  //on initialise un tableau pour stocker les poids totaux selon les dimensions
  int * listePoidsSolution = (int *)calloc(row->nbDimensions,sizeof(int));

  //on trie la liste selon la stratégie choisie
  tri_liste(row,list,strategie);
  
  //tant que la liste n'est pas vide
  while (j < list->nbElement){ 
    //stocke le nombre de poids d'un élément pouvant etre additionnés au poids totaux
    //sans dépasser les poids maximaux
    int nbPoidsRentrable = 0; 

   
    //on parcourt toutes le sdimensions 
    for (int dim = 0 ; dim < row->nbDimensions ; dim++){
      int poid_obj = (row->listeObjets + list->numero[j])->listePoids[dim];
      //si le poids de l'objet dans une dimension rentre dans le sac
      if((listePoidsSolution[dim] + poid_obj) <= row->poidsMax[dim]){
        //alors le poids est rentrable dans le sac
        nbPoidsRentrable++; 
      }
    
    }
    //si tous les poids de toutes les dimensions de l'objet rentrent dans le sac
    //(aucun poids d'aucune dimension ne dépasse le poids max respectif)
    if(nbPoidsRentrable == row->nbDimensions){
      //on ajoute tous les poids de l'objets dans toutes les dimensions aux poids totaux
      for (int dim = 0 ; dim < row->nbDimensions ; dim++){
        int poid_obj = (row->listeObjets + list->numero[j])->listePoids[dim];
        listePoidsSolution[dim] += poid_obj;
      }
      //on ajoute l'objet à la solution
      solution[list->numero[j]] = 1;
    }
    j++;
  }

  //on libère les mémoires allouées
  free(listePoidsSolution);
  list_destroy(list);
  return solution;
}