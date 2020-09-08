#ifndef PRAIRIE_H
#define PRAIRIE_H
#include "plague.h"

void initialiser_voisin_prairie(struct individu *individu, struct param_globaux *param);

double determiner_rayon_individu(struct param_globaux *param);

int distance_individus(struct param_globaux *param, struct individu *individu, int ind1, int ind2);

void mort_maj_voisin(struct individu *individu, struct param_globaux *param, int numero_individu);

#endif
