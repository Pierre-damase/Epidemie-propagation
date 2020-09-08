#include <stdio.h>
#include "anneau.h"
#include "plague.h"
#include "listechaine.h"

/**
 * Initialisation de l'anneau : chaque individu à deux voisins
 * @param individu
 * @param param
 */
void initialiser_voisin_anneau(struct individu *individu, struct param_globaux *param){
    for(int i = 0; i < param -> taille; i++){
        if(i == 0)
            individu[i].voisins = ajout_cell_debut(param -> taille - 1, individu[i].voisins);
        else if(i == param -> taille - 1){
            individu[i].voisins = ajout_cell_debut(0, individu[i].voisins);
            individu[i].voisins = ajout_cell_debut(i-1, individu[i].voisins);
            individu[i].nb_voisins = 2;
            break;
        }
        else
            individu[i].voisins = ajout_cell_debut(i-1, individu[i].voisins);
        individu[i].voisins = ajout_cell_debut(i+1, individu[i].voisins);
        individu[i].nb_voisins = 2;
    }
    //afficher_cell_voisins(individu,param->taille);
    return;
}
