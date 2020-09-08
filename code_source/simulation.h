#ifndef SIMULATION_H
#define SIMULATION_H
#include "plague.h"

double determiner_nb_moyen_voisin(struct param_globaux *param,struct individu *individu);

void update_etat(struct cell *pc, struct individu *individu,int mod);

void initialiser_voisin(struct individu *individu, struct param_globaux *param, struct cell *lStanford);

void initialiser_simulation(struct param_globaux *param, struct individu *individu,struct cell *lStanford);

void simulation(struct param_globaux *param, struct individu *individu, struct cell *lInfecte, struct cell *lResistant, struct cell *lMort,struct cell *lStanford);

int rechercher_nb_voisin_inf(struct individu *individu, int num_individu, struct cell *pc,type_etat etat);

int transition_etat(double proba, int nb_individu, int val,int mod);

struct cell *transition_infecte(struct individu *individu,struct param_globaux *param,struct cell *lInfecte,struct cell *lMort,int num_individu);

struct cell *transition_sain(struct individu *individu,struct param_globaux *param,struct cell *lResistant,int num_individu);

struct cell *transition_mort(struct individu *individu,struct param_globaux *param,struct cell *lInfecte,struct cell *lMort,int num_individu);

struct cell *transition_res(struct param_globaux *param,struct cell *lInfecte,struct cell *lResistant,int num_individu);

struct cell *transition_imun(struct individu *individu,struct param_globaux *param,struct cell *lResistant,int num_individu);

void affichage_simulation(struct individu *individu, struct param_globaux *param,struct cell *lInfecte,struct cell *lResistant, struct cell *lMort, struct cell *lStanford, int jour_sans_infecte, int nb_tour);

void ecrire_file(struct individu *individu, struct param_globaux *param, int nb_tou, int nb_inf, int nb_res, int nb_mort, int nb_sain,struct cell *lInfecte,struct cell *lResistant, struct cell *lMort,struct cell *lStanford);
#endif
