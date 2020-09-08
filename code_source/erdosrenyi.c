#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "erdosrenyi.h"
#include "plague.h"
#include "listechaine.h"

/**
 * Déterminer l'existence d'une arête entre deux individus à partir de
 * la probabilité p
 * @param individu
 * @param param
 * @return 0 l'arrête n'existe pas / 1 l'arrête existe
 */
int determiner_existence_arrete(struct param_globaux *param){
    double random = fmod(rand(),param->taille*100) /(param->taille);
    if(random >= (param->proba_existence_arete * 100))
        return 0;
    return 1;
}


/**
 * initialistaion de la génération aléatoire de erdos-rényi : il y a une
 * probabilité p d'existence d'une arête entre deux individus
 * @param individu
 * @param param
 */
void initialiser_voisin_erdosrenyi(struct individu *individu, struct param_globaux *param){
    for(int i = 0; i < param->taille; i++){
        for(int j = i+1; j < param->taille; j++){
            if(determiner_existence_arrete(param) == 1 && rechercher_cell_liste(individu[j].voisins,i) == 0){
                individu[i].voisins = ajout_cell_debut(j, individu[i].voisins);
                individu[j].voisins = ajout_cell_debut(i, individu[j].voisins);
            }
        }
        individu[i].nb_voisins = rechercher_nb_individu_cell(individu[i].voisins);
    }
    //afficher_cell_voisins(individu,param->taille);
    return;
}
