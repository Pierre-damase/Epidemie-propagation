#include <stdlib.h>
#include "clique.h"
#include "plague.h"
#include "listechaine.h"

/**
 * Initialisation de la clique : tout le monde connaît tout le monde
 * @param individu
 * @param param
 */
void initialiser_voisin_clique(struct individu *individu, struct param_globaux *param){
    for(int i = 0; i < param -> taille; i++){
        for(int j = 0; j < param -> taille; j++)
            if(j != i)
                individu[i].voisins = ajout_cell_debut(j, individu[i].voisins);
        individu[i].nb_voisins = param->taille-1;
    }
    //afficher_cell_voisins(individu,param->taille
    return;
}
