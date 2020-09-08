#ifndef LISTECHAINE_H
#define LISTECHAINE_H
#include "plague.h"


/**
 * @field clef numéro de l'individu
 * @field cpt gére le nombre de jour depuis l'infection ou la résistance
 * Dans la liste lStanford, contient le num réel de l'individu dans le fichier
 * @field suiv pointe sur la liste suivante
 */
struct cell{
    int clef;
    int cpt;
    struct cell *suiv;
};

struct cell *ajout_cell_debut(int num_voisin, struct cell *pc);

struct cell *suppression_cell_liste(struct cell *pc, struct cell *preced , int num_voisin);

void liberer_cell(struct cell *pc);

void afficher_cell_liste(struct cell *pc,struct cell *lStanford);

void ecrire_cell_liste(FILE *file,struct cell *pc,struct cell *lStanford);

int rechercher_cell_liste(struct cell *pc, int num_voisin);

int rechercher_cell_stanford(struct cell *pc,int num_sommet);

int rechercher_cell_compteur(struct cell *pc, int num_individu);

int rechercher_nb_individu_cell(struct cell *pc);

struct cell *initialiser_cell_debut(struct cell *pc, struct individu *individu, struct param_globaux *param, int mod);


int select_random_cell(struct cell *pc);

int rechercher_clef_cell(struct cell *pc, int val);

struct cell *vider_cell(struct cell *pc);


void afficher_cell_voisins(struct individu *individu, int taille);
#endif
