#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "listechaine.h"
#include "plague.h"


/**
 * Libère la mémoire allouée à liste chaînée
 * @param pc
 */
void liberer_cell(struct cell *pc){
    if(pc == NULL)
        return;
    liberer_cell(pc -> suiv);
    free(pc);
}


/**
 * Supprime une cellule de la liste chaînée
 * @param pc
 * @param preced
 * @param num_voisin
 * @return pc
 */
struct cell *suppression_cell_liste(struct cell *pc,struct cell *preced,int num_voisin){
    struct cell *pcs = NULL;
    if(pc == NULL){
        printf("suppression : liste vide !\n");
        exit(EXIT_FAILURE);
    }
    if(pc -> clef == num_voisin){
        if(preced == NULL){
            pcs = pc -> suiv;
            free(pc);
            return pcs;
        }
        else {
            preced -> suiv = pc -> suiv;
            free(pc);
        }
    }
    else
        suppression_cell_liste(pc -> suiv, pc, num_voisin);

    return pc;
}


/**
 * Ajoute une cellule au début de la liste chaînée
 * @param num_voisin
 * @param pc
 * @return pc
 */
struct cell *ajout_cell_debut(int num_voisin,struct cell *pc){
    struct cell *pnc;
    pnc = (struct cell*) malloc(sizeof(struct cell));
    if(pnc == NULL)
        err(EXIT_FAILURE,"error malloc ajout_cell_debut");
    pnc -> clef = num_voisin;
    pnc -> cpt = 0;
    pnc -> suiv = pc;
    return pnc;
}


/**
 * Recherche la présence d'un élément dans une liste chaînée
 * @param pc
 * @param num_voisin
 * @return 1 l'élément est présent / 0 l'élément est absent
 */
int rechercher_cell_liste(struct cell *pc,int num_voisin){
    while(pc != NULL){
        if(pc -> clef == num_voisin)
            return 1;
        pc = pc -> suiv;
    }
    return 0;
}


/**
 * Renvoie le numéro l'indice d'un sommet
 * @param pc
 * @param num_sommet
 * @return num ou -1
 */
int rechercher_cell_stanford(struct cell *pc,int num_sommet){
    while(pc != NULL){
        if(pc -> cpt == num_sommet)
            return pc->clef;
        pc = pc -> suiv;
    }
    return -1;
}


/**
 * Rechercher la date depuis l'infection ou la résistance d'un individu
 * Cas lStanford, renvoie le num réelle d'un individu dans le fichier
 * associé à son indice dans le tableau d'individu
 * @param pc
 * @param num_individu
 * @return cpt ou 0
 */
int rechercher_cell_compteur(struct cell *pc,int num_individu){
    while(pc != NULL){
        if(pc -> clef == num_individu)
            return pc -> cpt;
        pc = pc -> suiv;
    }
    return 0;
}


/**
 * Détermine le nombre d'individu d'une liste chaînée
 * @param pc
 * @return cpt
 */
int rechercher_nb_individu_cell(struct cell *pc){
    int cpt = 0;
    while(pc != NULL){
        cpt++;
        pc = pc -> suiv;
    }
    return cpt;
}


/**
 * Affiche une liste chaînée
 * @param pc
 * @param lStanford
 */
void afficher_cell_liste(struct cell *pc,struct cell *lStanford){
    printf("[ ");
    while(pc != NULL){
        if(lStanford != NULL) // cas stanford
            printf("%d", rechercher_cell_compteur(lStanford,pc->clef));
        else
            printf("%d", pc -> clef);
        pc = pc -> suiv;
        if(pc != NULL)
            printf(", ");
        else
            printf(" ");
    }
    printf("]\n\n");
    return;
}


/**
 * Écrire une liste chaînée dans un fichier
 * @param pc
 * @param lStanford
 */
void ecrire_cell_liste(FILE *file,struct cell *pc,struct cell *lStanford){
    fprintf(file,"[ ");
    while(pc != NULL){
        if(lStanford != NULL) // cas stanford
            fprintf(file,"%d", rechercher_cell_compteur(lStanford,pc->clef));
        else
            fprintf(file,"%d", pc -> clef);
        pc = pc -> suiv;
        if(pc != NULL)
            fprintf(file,", ");
        else
            fprintf(file," ");
    }
    fprintf(file,"]\n\n");
    return;
}


/**
 * Initialise la liste des infectés/résistants et l'état des individus
 * initialement contaminés/résistants
 * @param pc
 * @param individu
 * @param param
 * @param mod
 * @return pc
 */
struct cell *initialiser_cell_debut(struct cell *pc, struct individu *individu, struct param_globaux *param, int mod){
    int random = 0,taille;
    if(mod == 0) taille = param->nb_patient_zero;
    else taille = param->nb_resistant_debut;
    for(int i = 0; i < taille; i++){
        while(1){
            random = rand() % param->taille;
            if(individu[random].etat == SAIN){
                if(mod == 0) individu[random].etat = INF;
                else individu[random].etat = RES;
                pc = ajout_cell_debut(random, pc);
                pc->cpt += 1;
                break;
            }
        }
    }
    return pc;
}


/**
 * Sélectionne une cellule aléatoire parmi celle de la liste chaînée et
 * renvoie sa clef
 * @param pc
 * @return clef
 */
int select_random_cell(struct cell *pc){
    int random = rand() % rechercher_nb_individu_cell(pc);
    for(int i = 0; i < random; i++)
        pc = pc->suiv;
    return pc->clef;
}


/**
 * Renvoie la clef d'un individu de la liste chaînée
 * @param pc
 * @param val
 * @return clef
 */
int rechercher_clef_cell(struct cell *pc, int val){
    for(int i = 0; i < val; i++)
        pc = pc -> suiv;
    return pc -> clef;
}


/**
 * Supprime l'ensemble des éléments d'une liste chaînée
 * @param pc
 * @return pc
 */
struct cell *vider_cell(struct cell *pc){
    do{
        pc = suppression_cell_liste(pc,NULL,select_random_cell(pc));
    } while(pc != NULL);
    return pc;
}


/**
 * Affiche la liste chaînée de voisins de chaque individu de la simulation
 * @param individu
 * @param taille
 */
void afficher_cell_voisins(struct individu *individu, int taille){
    printf("\n");
    for(int i = 0; i < taille; i++){
        printf("Individu %d à %d voisins : ",i,individu[i].nb_voisins);
        if(taille <= 70)
            afficher_cell_liste(individu[i].voisins,NULL);
    }
}
