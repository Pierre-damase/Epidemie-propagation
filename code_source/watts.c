#include <stdlib.h>
#include <err.h>
#include "watts.h"
#include "plague.h"
#include "listechaine.h"

/**
 * Initialise les voisins selon l'anneau de Watts-Strogatz
 * @param individu
 * @param param
 */
void initialiser_voisin_watts(struct individu *individu, struct param_globaux *param){
    int tmp = 0;
    for(int i = 0; i < param->taille; i++){
        individu[i].num = i;
        for(int j = i+1; j <= i+param->nb_rebranchement; j++){
            tmp = j % param->taille;
            individu[i].voisins = ajout_cell_debut(tmp, individu[i].voisins);
            individu[tmp].voisins = ajout_cell_debut(i,individu[tmp].voisins);
        }
    }
    verifier_nombre_arete(individu,param); // vérification avant rebranchement

    affichage_watts(individu,param);

    rebranchement_watts(individu,param);
    verifier_nombre_arete(individu,param); // vérification après rebranchement

    affichage_watts(individu,param);
    return;
}


/**
 * Affiche les voisins de chaque individu
 * @param individu
 * @param param
 */
void affichage_watts(struct individu *individu, struct param_globaux *param){
    for(int i = 0; i < param->taille; i++)
        individu[i].nb_voisins = rechercher_nb_individu_cell(individu[i].voisins);
    //afficher_cell_voisins(individu,param->taille);
}


/**
 * Dans le modèle de Watts-Strogatz, le nombre d'arêtes est m = k*n
 * Permet de vérifier que le nombre d'arêtes m n'a pas été modifié
 * après rebranchement
 * @param individu
 * @param param
 */
void verifier_nombre_arete(struct individu *individu, struct param_globaux *param){
    int nb_aretes = 0, m = param->nb_rebranchement*param->taille;
    for(int i = 0; i < param->taille; i++)
        nb_aretes += rechercher_nb_individu_cell(individu[i].voisins);
    if(nb_aretes/2 != m){
        liberer_tableau(individu,param);
        free(param);
        err(EXIT_FAILURE,"initialiser voisin, nombre d'arêtes différent de k*n = %d\n",m);
    }
    return;
}


/**
 * Si nouveau_voisin renvoie une valeur différente de -1
 * Supprime l'arête actuelle
 * Fait le rebranchement entre l'individu et sa nouvelle extrémité
 * @param individu
 * @param param
 */
void rebranchement_watts(struct individu *individu, struct param_globaux *param){
    int cpt = 1, tmp = 0, nouveau_voisin;
    for(int k = 0; k < param->nb_rebranchement; k++){
        for(int i = 0; i < param->taille; i++){
            nouveau_voisin = selection_nouveau_sommet(individu,param,i);
            if(nouveau_voisin == -1)
                continue;
            tmp = (i+cpt) % param->taille;
            individu[i].voisins = suppression_cell_liste(individu[i].voisins,NULL,tmp);
            individu[tmp].voisins = suppression_cell_liste(individu[tmp].voisins,NULL,i);
            individu[i].voisins = ajout_cell_debut(nouveau_voisin,individu[i].voisins);
            individu[nouveau_voisin].voisins = ajout_cell_debut(i,individu[nouveau_voisin].voisins);
        }
        cpt++;
    }

    return;
}


/**
 * Probabilité de rebranchement indiquée dans le fichier de configuration
 * Détermine l'ensemble des sommets éligibles à devenir l'autre
 * extrémité de l'arête num_individu
 * Détermine s'il y a un rebranchement entre num_individu et un des
 * sommets éligibles
 * @param individu
 * @param param
 * @param num_individu
 * @return -1 pas de rebranchement / valeur de la nouvelle extrémité
 */
int selection_nouveau_sommet(struct individu *individu, struct param_globaux *param, int num_individu){
    struct cell *lSelection = NULL;
    int random = 0, size = 0, nouveau_voisin = -1;
    for(int i = 0; i < param->taille; i++)
        if(i != num_individu && rechercher_cell_liste(individu[num_individu].voisins,i) == 0)
            lSelection = ajout_cell_debut(i,lSelection);
    size = rechercher_nb_individu_cell(lSelection);

    for(int i = 0; i < size; i++){
        random = (rand() % (size*100)) / size;
        if((double)random < param->proba_rebranchement*100){
            nouveau_voisin = rechercher_clef_cell(lSelection,i);
            break;
        }
    }
    liberer_cell(lSelection);

    return nouveau_voisin;
}
