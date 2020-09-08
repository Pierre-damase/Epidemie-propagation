#ifndef BARABASI_H
#define BARABASI_H
#include "plague.h"

struct individu *initialiser_barabasi(struct individu *individu, struct param_globaux *param);

struct individu *initialiser_voisin_barabasi(struct individu *individu, struct param_globaux *param);

int connexion_sommet(struct individu *individu, struct param_globaux *param, int num_individu, int num_voisin, int val, int somme_degre);

struct param_globaux *maj_taille_barabasi(struct param_globaux *param);
#endif
