#include <stdlib.h>
#include <err.h>
#include "barabasi.h"
#include "plague.h"
#include "listechaine.h"
#include "clique.h"
#include "simulation.h"


/**
 * Initialise le modèle de barabasi-albert
 * @param individu
 * @param param
 * @return individu
 */
struct individu *initialiser_barabasi(struct individu *individu, struct param_globaux *param){

    initialiser_voisin_clique(individu,param); // modèle de départ, une clique de n0 individus
    param->degre_barabasi = rechercher_nb_individu_cell(individu[0].voisins); // initilisation du paramètre d
    if(param->sommet_barabasi == param->taille)
        return individu;

    individu = (struct individu *) realloc(individu,param->sommet_barabasi*sizeof(struct individu));
    if(individu == NULL){
        liberer_tableau(individu,param);
        free(param);
        err(EXIT_FAILURE,"initialiser_voisin_barabasi : realloc null");
    }

    individu = initialiser_voisin_barabasi(individu,param);
    //afficher_cell_voisins(individu,param->sommet_barabasi);

    return individu;
}


/**
 * Initialise les voisins selon le modèle de barabasi-albert
 * @param individu
 * @param param
 * @return individu
 */
struct individu *initialiser_voisin_barabasi(struct individu *individu, struct param_globaux *param){
    struct cell *lSelection = NULL;
    int num_voisin,somme_degre, cpt;

    for(int i = param->taille; i < param->sommet_barabasi; i++){
        cpt = 0;
        individu[i].num = i;
        individu[i].etat = SAIN;
        individu[i].vaccin = NV;
        individu[i].nb_voisins = 0;
        individu[i].voisins = NULL;
        for(int j = 0; j < i; j++)
            lSelection = ajout_cell_debut(j,lSelection); // tous les sommets avec lequel i peut devenir voisins

        for(int j = 0; j < param->degre_barabasi; j++){ // 
            somme_degre = 0;

            num_voisin = select_random_cell(lSelection); // sélection d'un des sommets parmi ceux éligibles
            lSelection = suppression_cell_liste(lSelection,NULL,num_voisin); // supprimer de la liste le sommet sélectionné

            for(int k = 0; k <= i; k++)
                somme_degre += individu[k].nb_voisins; // somme de tous les noeuds existant dans le graphe

            if(connexion_sommet(individu,param,i, num_voisin, cpt, somme_degre) == 0)
                continue; // i et num_voisin pas connectés entre eux

            individu[i].voisins = ajout_cell_debut(num_voisin,individu[i].voisins);
            individu[num_voisin].voisins = ajout_cell_debut(i,individu[num_voisin].voisins);

            individu[i].nb_voisins++;
            individu[num_voisin].nb_voisins++;
            cpt++;
        }
        lSelection = vider_cell(lSelection);
    }
    liberer_cell(lSelection);

    return individu;
}


/**
 * Détermine si le sommet num_individu ajouté est connecté au sommet
 * num_voisin avec une probabilité pi suivant la loi de degré
 * @param individu
 * @param param
 * @return 1 connexion / 0 pas de connexion
 */
int connexion_sommet(struct individu *individu, struct param_globaux *param, int num_individu, int num_voisin, int val, int somme_degre){
    if(rechercher_cell_liste(individu[num_individu].voisins,num_voisin) == 1)
        return 0;

    double random = ((double)rand() / RAND_MAX) * (100);
    double proba = ((double)individu[num_voisin].nb_voisins / (somme_degre-val*param->degre_barabasi))*100;

    if(random >= proba) return 0;
    return 1;
}


/**
 * Après ajout des sommets, met à jour la taille du graphe
 * @param individu
 * @param param
 * @return param
 */
struct param_globaux *maj_taille_barabasi(struct param_globaux *param){
    param->taille = param->sommet_barabasi;
    return param;
}
