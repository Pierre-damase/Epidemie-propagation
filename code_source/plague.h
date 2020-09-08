#ifndef PLAGUE_H
#define PLAGUE_H
#include <stdio.h>

#define SIZE 64

/**
 * @field NV non vacciné
 * @field IMUM immunisé
 */
typedef enum {NV, IMUN} type_vaccin;

/**
 * État de l'individu dans la simulation
 * @field SAIN
 * @field INF infecté
 * @field RES resistant
 * @field MORT
 */
typedef enum {SAIN, INF, RES, MORT} type_etat;

/**
 * Les différentes topologies applicables à la simulation
 * @field CLIQUE
 * @field ANNEAU
 * @field ERDOS_RENYI
 * @field STANFORD
 * @field PRAIRIE
 * @field WATTS
 * @field BARABASI
 */
typedef enum {CLIQUE, ANNEAU, ERDOS_RENYI, STANFORD, PRAIRIE, WATTS, BARABASI} type_topologie;


/**
 * Contient l'ensemble des individus
 * @field num numéro de l'individu
 * @field etat état de l'individu
 * @field vaccin individu vacciné ou non
 * @field rayon_prairie rayon de l'individu dans le modèle de la prairie
 * @field position_prairie position de l'individu dans le modèle
 * de la prairie
 * @field nb_voisins nombre de voisins de l'individu
 * @field voisins liste chainée de voisins de l'individu
 */
struct individu {
    int num;
    type_etat etat;
    type_vaccin vaccin;
    double rayon_prairie;
    int position_prairie;
    int nb_voisins;
    struct cell *voisins; // liste de voisin
};


/**
 * Contient les paramètres globaux de la simulation
 * chargés depuis el fichier de configuration
 */
struct param_globaux{
    int taille; // nb total individus
    double proba_c; // S -> C
    double proba_r; // C -> R
    double proba_s; // R -> S
    double proba_m; // C -> M
    type_topologie topologie;
    int nb_patient_zero;
    int nb_resistant_debut;
    int nb_tour;
    double proba_existence_arete; // modèle erdos-renyi
    char stanford_file[SIZE]; // fichier stanford à lire
    int stanford_arrete; // nb arrête dans le fichier stanford
    int stanford_nmax; // valeur max d'une arrête
    double vitesse_propagation; // vitesse de propagation de l'infection
    int taille_prairie; // modèle prairie aléatoire
    double rayon_max; // rayon maximum
    double rayon_min; // rayon minimum
    double augmenter_rayon; // augmentation du rayon à la mort d'un individu
    double proba_v; // S -> V
    int jour_vaccination; // jour de la vaccination
    double proba_rebranchement;
    int nb_rebranchement;
    int sommet_barabasi;
    int degre_barabasi;
    char file_output[SIZE];
    char file_simulation[SIZE];
};


struct individu *ini_individu(struct param_globaux *param);

void liberer_tableau(struct individu *individu, struct param_globaux *param);

struct param_globaux *lecture_fichier(struct param_globaux *param, const char *file);

void menu(struct param_globaux *param);
void affichage_menu(struct param_globaux *param);

FILE *open_file(const char *file,struct param_globaux *param);
FILE *write_file(const char *file,struct param_globaux *param, int mod);

void ecrire_voisins_file(struct individu *individu, struct param_globaux *param,struct cell *lStanford);
#endif
