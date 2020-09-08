#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <time.h>
#include "plague.h"

int main (){
    srand(time(NULL));
    int mod = -1, tmp = 0, simulation = 0;
    struct param_globaux *parametre = NULL;

    while(tmp == 0){
        printf("Bienvenue dans la simulation de la propagation d'une maladie!\n\n");
        printf("1. Renseigner le fichier de configuration à lire\n");
        printf("2. Quitter le programme\n\n");
        printf("Choix : ");
        if(scanf("%d",&mod) != 1)
            err(EXIT_FAILURE,"scanf main : valeur autre qu'un integer renseignée\n");

        switch(mod){
        case 1:
            parametre = malloc(sizeof(struct param_globaux));
            printf("Fichier à lire : ");
            scanf("%s",parametre->file_simulation);
            parametre = lecture_fichier(parametre,parametre->file_simulation);
            menu(parametre);
            free(parametre);
            simulation = 1;
            break;
        case 2:
            tmp = 1;
            break;
        default:
            printf("Veuillez faire une sélection (1 ou 2)\n");
            if(simulation == 0) free(parametre);
            break;
        }
    }
    if(simulation == 0) free(parametre);
    return 0;
}
