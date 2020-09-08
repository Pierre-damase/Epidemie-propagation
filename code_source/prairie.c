#include <stdlib.h>
#include <math.h>
#include "prairie.h"
#include "plague.h"
#include "listechaine.h"

/**
 * Initialiser le graphe de disque aléatoire
 * Chaque individu se voit affecter une position sélectionnée de façon
 * aléatoire parmi l'ensemble des positions disponibles
 * @param individu
 * @param param
 */
void initialiser_voisin_prairie(struct individu *individu, struct param_globaux *param){
    struct cell *lPosition = NULL;
    int dimension = param->taille_prairie*param->taille_prairie;

    for(int i = 0; i < dimension; i++)
        lPosition = ajout_cell_debut(i,lPosition); // contient toutes les positions de la prairie

    for(int i = 0; i < param->taille; i++){ // initialiser position dans la prairie de chaque individu
        individu[i].position_prairie = select_random_cell(lPosition);
        individu[i].rayon_prairie = determiner_rayon_individu(param);
        lPosition = suppression_cell_liste(lPosition,NULL,individu[i].position_prairie);
    }
    liberer_cell(lPosition);

    for(int i = 0; i < param->taille; i++){ // initialiser les voisins
        for(int j = i+1; j < param->taille; j++){
            if(distance_individus(param,individu,i,j) == 1){
                individu[i].voisins = ajout_cell_debut(j,individu[i].voisins);
                individu[j].voisins = ajout_cell_debut(i,individu[j].voisins);
            }

        }
        individu[i].nb_voisins = rechercher_nb_individu_cell(individu[i].voisins);
    }
    //afficher_cell_voisins(individu,param->taille);
    return;
}


/**
 * Détemine le rayon de chaque individu : compris enter rmin et rmax
 * @param param
 * @return rayon de l'individu dans la prairie
 */
double determiner_rayon_individu(struct param_globaux *param){
    return ((double)rand() / RAND_MAX) * (param->rayon_max - param->rayon_min) + param->rayon_min;
}


/**
 * Détermine si deux individus sont voisins
 * Converti leurs positions en coordonnées x et y
 * Calcul de la distance euclidienne entre les deux individus
 * Si la somme de leurs rayons > distance alors ils sont voisins
 * @param param
 * @param individu
 * @param individu 1
 * @param individu 2
 * @return 1 s'ils sont voisins / 0 s'ils ne le sont pas
 */
int distance_individus(struct param_globaux *param, struct individu *individu, int ind1, int ind2){
    int x_ind1 = individu[ind1].position_prairie % param->taille_prairie,
        x_ind2 = individu[ind2].position_prairie % param->taille_prairie,
        y_ind1 = individu[ind1].position_prairie / param->taille_prairie,
        y_ind2 = individu[ind2].position_prairie / param->taille_prairie;
    double distance = sqrt(pow(x_ind2-x_ind1,2) + pow(y_ind2-y_ind1,2));
    double rayon = individu[ind1].rayon_prairie + individu[ind2].rayon_prairie;

    if(rayon > distance)
        return 1;
    return 0;
}


/**
 * Le rayon des individus qui meurent peut augmenter selon une valeur
 * renseignée dans le fichier de configuration
 * L'augmentation du rayon entraîne la création du nouveau lien
 * Cette fonction sert à rajouter les liens entre individus s'ils
 * deviennent voisins
 * @param individu
 * @param param
 * @param numero_individu : numéro de l'individu mort
 */
void mort_maj_voisin(struct individu *individu, struct param_globaux *param, int numero_individu){
    for(int i = 0; i < param->taille-1; i++){ // initialiser voisin
        if(i != numero_individu && distance_individus(param,individu,i,numero_individu) == 1 && rechercher_cell_liste(individu[numero_individu].voisins,i) == 0){
            individu[i].voisins = ajout_cell_debut(numero_individu,individu[i].voisins);
            individu[numero_individu].voisins = ajout_cell_debut(i,individu[numero_individu].voisins);
            individu[i].nb_voisins++;
            individu[numero_individu].nb_voisins++;
        }
    }
    return;
}
