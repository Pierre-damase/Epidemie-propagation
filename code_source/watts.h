#ifndef WATTS_H
#define WATTS_H
#include "plague.h"

void initialiser_voisin_watts(struct individu *individu, struct param_globaux *param);

void rebranchement_watts(struct individu *individu, struct param_globaux *param);

int selection_nouveau_sommet(struct individu *individu, struct param_globaux *param, int num_individu);

void verifier_nombre_arete(struct individu *individu, struct param_globaux *param);

void affichage_watts(struct individu *individu,struct param_globaux *param);
#endif
