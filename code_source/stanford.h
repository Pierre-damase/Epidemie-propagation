#ifndef STANFORD_H
#define STANFORD_H
#include "plague.h"

void initialiser_voisin_stanford(struct individu *individu, struct param_globaux *param,struct cell *lStanford);

struct cell *determiner_parametre_stanford(struct param_globaux *param,struct cell *lStanford);

void ecrire_stanford_format(struct individu *individu,struct param_globaux *param, struct cell *lStanford);
#endif
