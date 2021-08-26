#include <stdio.h>
#include <stdlib.h>
#include "parseur.h"



/** fonction qui prend en entrée un fichier d'instances formaté et qui créer une structure instanceDB associée
*@param : char* filename : le nom du fichier que l'on veut parser
*@return : instanceDB * row : la structure instanceDB contenant toute les instances du fichier
*/
instanceDB * parser(char* filename)
{
	  int nbInstance = 0;

    /** definition des fichier d'entrée et sortie */
    FILE* fichier = NULL;

    /**ouverture du fichier d'entrée */
    fichier = fopen(filename,"r");
    if(fichier==NULL){
        perror("erreur d'ouverture");
        exit(EXIT_FAILURE);
    }

    /** lecture du nombre d'instance */
    fscanf(fichier, "%d", &nbInstance);

    instanceDB * row = (instanceDB*)malloc(sizeof(instanceDB));
    row->nbInstances = nbInstance;
    row->listeInstances = (instance **)malloc((size_t)(row->nbInstances)*sizeof(instance*));

    /** pour chaque instance on lit l'instance */
    for(int i=0; i<nbInstance; i++){
        /** instancie une instance */
        row->listeInstances[i] = readInstance(fichier);
    }

    fclose(fichier);
    return row;
}
