#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <err.h>
#include <time.h>
#include "plague.h"
#include "simulation.h"
#include "listechaine.h"
#include "stanford.h"
#include "barabasi.h"

#define SIZE 64

/**
 * Initialise le tableau d'individu par défaut
 * Chaque individu est sain et non vacciné
 * @param param
 * @return individu
 */
struct individu *ini_individu(struct param_globaux *param){
    struct individu *tab = NULL;
    tab = (struct individu*) calloc(param -> taille,sizeof(struct individu));
    if(tab == NULL)
        err(EXIT_FAILURE,"error malloc ini_individu");

    for(int i = 0; i < param -> taille; i++){
        tab[i].etat = SAIN;
        tab[i].vaccin = NV;
        tab[i].num = i;
        tab[i].rayon_prairie = 0.0;
        tab[i].nb_voisins = 0;
    }
    return tab;
}


/**
 * Ouvre un fichier en lecture seulement
 * @param file
 * @param param
 * @return file
 */
FILE *open_file(const char *file,struct param_globaux *param){
    FILE *fd = fopen(file, "r");
    if(fd == NULL){
        free(param);
        err(EXIT_FAILURE, "lecture fichier (open_file) : error\n");
    }
    return fd;
}


/**
 * Ouvre un fichier en écriture seulement
 * Mod 0 : créer le fichier s'il existe pas et écrit depuis le début du fichier
 * Mod 1 : créer le fichier s'il existe pas et écrit en mode ajout
 * @param file
 * @param param
 * @return file
 */
FILE *write_file(const char *file,struct param_globaux *param, int mod){
    FILE *fd = NULL;
    if(mod == 0)
        fd = fopen(file, "w");
    else
        fd = fopen(file, "a");
    if(fd == NULL){
        free(param);
        err(EXIT_FAILURE, "écriture fichier (write_file) : error\n");
    }
    return fd;
}


/**
 * Libère la mémoire allouée aux listes de voisins puis au tableau
 * d'individu
 * @param individu
 * @param param
 */
void liberer_tableau(struct individu *individu, struct param_globaux *param){
    for(int i = 0; i < param -> taille; i++)
        liberer_cell(individu[i].voisins);
    if(param->topologie == BARABASI)
        for(int i = param->taille; i < param->sommet_barabasi; i++)
            liberer_cell(individu[i].voisins);
    free(individu);
}


/**
 * Menu pour lancer la simulation
 * @param param
 */
void menu(struct param_globaux *param){
    int mod = -1;
    struct individu *individu = NULL;
    struct cell *lStanford = NULL;
    if(param->topologie == STANFORD)
        lStanford = determiner_parametre_stanford(param,lStanford);

    affichage_menu(param);
    printf("1. Lancer la simulation\n");
    printf("2. Arrêter la simulation\n\n");
    printf("Choix : ");
    if(scanf("%d", &mod) != 1){
        free(param);
        liberer_cell(lStanford);
        err(EXIT_FAILURE, "scanf menu : valeur autre qu'un integer renseignée\n");
    }

    switch(mod){
    case 1:
        printf("\nNom du fichier de sorti (/dev/null pour empécher écriturex) : ");
        scanf("%s",param->file_output);

        individu = ini_individu(param);
        if(param->topologie != BARABASI)
            initialiser_voisin(individu, param, lStanford);
        else{
            individu = initialiser_barabasi(individu,param);
            param = maj_taille_barabasi(param);
        }
        ecrire_voisins_file(individu,param,lStanford);
        initialiser_simulation(param, individu, lStanford);

        FILE *fd = write_file(param->file_output,param,1);
        if(param->topologie == STANFORD){
            printf("\nLes paramètres du graphe sont nmax : %d, nombre effectif de sommets : %d, nombre d'arrêtes : %d, degré moyen : %.2lf\n",param->stanford_nmax,param->taille,param->stanford_arrete,determiner_nb_moyen_voisin(param,individu));
            fprintf(fd,"\nLes paramètres du graphe sont nmax : %d, nombre effectif de sommets : %d, nombre d'arrêtes : %d, degré moyen : %.2lf\n",param->stanford_nmax,param->taille,param->stanford_arrete,determiner_nb_moyen_voisin(param,individu));
        }
        else{
            printf("\nLes paramètres du graphe sont nmax : %d, degré moyen : %.2lf\n",param->taille,determiner_nb_moyen_voisin(param,individu));
            fprintf(fd,"\nLes paramètres du graphe sont nmax : %d, degré moyen : %.2lf\n",param->taille,determiner_nb_moyen_voisin(param,individu));
        }
        printf("La vitesse de propagation est : %lf\n",param->vitesse_propagation/param->nb_tour);
        fprintf(fd,"La vitesse de propagation est : %lf\n",param->vitesse_propagation/param->nb_tour);

        fclose(fd);
        ecrire_stanford_format(individu,param,lStanford);
        liberer_cell(lStanford);
        liberer_tableau(individu,param);
        return;
    case 2:
        liberer_cell(lStanford);
        return;
    default:
        printf("Erreur\n\n");
        menu(param);
        break;
    }
}


/**
 * Écrit dans un fichier texte les voisins de chaque individu
 * @param individu
 * @param param
 * @param lStanford
 */
void ecrire_voisins_file(struct individu *individu, struct param_globaux *param,struct cell *lStanford){
    static const char* type_topologie[] = {"Clique", "Anneau", "Erdos-Rényi", "Stanford", "Prairie aléatoire", "Modèle de Watts-Strogatz", "Modèle de Barabasi-Albert"};
    if(strcmp(param->file_output,"/dev/null") == 0)
        return;
    FILE *fd = write_file(param->file_output,param,0);

    fprintf(fd,"Simulation %s lancée avec la topologie %s : \n\n",param->file_simulation,type_topologie[param->topologie]);

    for(int i = 0; i < param->taille; i++){
        fprintf(fd,"Individu %d a %d voisin : ",i,individu[i].nb_voisins);
        ecrire_cell_liste(fd,individu[i].voisins,lStanford);
    }

    fclose(fd);
    return;
}


/**
 * Affiche les paramètres de la simulation chargés à partir
 * du fichier de configuration
 * @param param
 */
void affichage_menu(struct param_globaux *param){
    static const char* type_topologie[] = {"Clique", "Anneau", "Erdos-Rényi", "Stanford", "Prairie aléatoire", "Modèle de Watts-Strogatz", "Modèle de Barabasi-Albert"};

    printf("\n\nParamètres de la simulation :\n");
    printf("Nombre de tour : %d\n", param->nb_tour);
    printf("Taille population : %d\n", param->taille);
    printf("Probabilite infection : %.2lf\n", param->proba_c);
    printf("Probabilite de résistance : %.2lf\n", param->proba_r);
    printf("Probabilite de perte de résistance : %.2lf\n",param->proba_s);
    printf("Probabilite de mourir : %.2lf\n",param->proba_m);
    printf("Topologie : %s\n", type_topologie[param->topologie]);

    if(param->jour_vaccination > 0)
        printf("Campagne de vaccination le %dème jour avec une proabbilité de devenir resistant de %.2lf\n",param->jour_vaccination,param->proba_v);

    printf("Nombre de patient zéro : %d\n", param->nb_patient_zero);
    printf("Nombre d'individus résistant au début : %d\n\n", param->nb_resistant_debut);

    if(param->topologie == ERDOS_RENYI){
        printf("Probabilité p d'existence de chaque arête : %.2lf\n",param->proba_existence_arete);
        if(param->proba_existence_arete == 0){
            free(param);
            err(EXIT_FAILURE,"Graphe sans arête. L'épidémie ne peut pas se propager\n\n");
        }
        else if(param->proba_existence_arete == 1){
            param->topologie = 0;
            printf("On est dans le cas d'une clique\n\n");
        }
    }
    if(param->topologie == STANFORD)
        printf("Fichier Stanford à lire : %s\n\n", param->stanford_file);

    if(param->topologie == PRAIRIE){
        if(param->taille > pow(param->taille_prairie,2)){
            free(param);
            err(EXIT_FAILURE,"Erreur la taille de la prairie est inférieur au nombre d'individus\n\n");
        }
        else
            printf("Les paramètres de la prairie sont rayon minimun : %.2lf, rayon maximum %.2lf, taille %.0lf et l'augmentation du rayon des morts vaut %.2lf\n\n",param->rayon_min,param->rayon_max,pow(param->taille_prairie,2),param->augmenter_rayon);
    }

    if(param->topologie == WATTS)
        printf("Les paramètres du modèle sont n : %d, k : %d et p : %.2lf\n\n",param->taille,param->nb_rebranchement,param->proba_rebranchement);

    if(param->topologie == BARABASI){
        if(param->sommet_barabasi < param->taille){
            free(param);
            err(EXIT_FAILURE,"n doit être défini tel que n0(taille) <= n");
        }
        else
            printf("Les paramètres du modèle sont n0 : %d, n : %d\n",param->taille,param->sommet_barabasi);
    }
    return;
}



/**
 * Recupère les paramètres de la simulation à partir d'un fichier
 * de configuration et les stockes dans param
 * @param param
 * @param file
 * @return param
 */
struct param_globaux *lecture_fichier(struct param_globaux *param,const char *file){
    char buf[SIZE];
    int c, val;
    FILE *fd = open_file(file,param);

    while(1){
        c = fgetc(fd);
        if(c == EOF)
            break;
        else if(c == '#'){ // passer les éventuels commentaires
            do{
                c = fgetc(fd);
            } while(c != '\n');
        }
        else if(c != '\n'){
            fseek(fd,-1,SEEK_CUR);
            fscanf(fd,"%s",buf);
            if(strcmp(buf,"NB_TOUR") == 0)                     fscanf(fd,"%d",&param->nb_tour);
            else if(strcmp(buf,"NB_INDIVIDUS") == 0)           fscanf(fd,"%d",&param->taille);
            else if(strcmp(buf,"PROBA_C") == 0)                fscanf(fd,"%lf",&param->proba_c);
            else if(strcmp(buf,"PROBA_R") == 0)                fscanf(fd,"%lf",&param->proba_r);
            else if(strcmp(buf,"PROBA_S") == 0)                fscanf(fd,"%lf",&param->proba_s);
            else if(strcmp(buf,"NB_PATIENT_ZERO") == 0)        fscanf(fd,"%d",&param->nb_patient_zero);
            else if(strcmp(buf,"PROBA_M") == 0)                fscanf(fd,"%lf",&param->proba_m);
            else if(strcmp(buf,"NB_RES_DEBUT") == 0)           fscanf(fd,"%d",&param->nb_resistant_debut);
            else if(strcmp(buf,"PROBA_EXISTENCE_ARETE") == 0)  fscanf(fd,"%lf",&param->proba_existence_arete);
            else if(strcmp(buf,"STANFORD_FILE") == 0)          fscanf(fd,"%s",param->stanford_file);
            else if(strcmp(buf,"PRAIRIE_COTE") == 0)           fscanf(fd,"%d",&param->taille_prairie);
            else if(strcmp(buf,"RAYON_MAX") == 0)              fscanf(fd,"%lf",&param->rayon_max);
            else if(strcmp(buf,"RAYON_MIN") == 0)              fscanf(fd,"%lf",&param->rayon_min);
            else if(strcmp(buf,"RAYON_MORT") == 0)             fscanf(fd,"%lf",&param->augmenter_rayon);
            else if(strcmp(buf,"PROBA_VACCINATION") == 0)      fscanf(fd,"%lf",&param->proba_v);
            else if(strcmp(buf,"JOUR_VACCINATION") == 0)       fscanf(fd,"%d",&param->jour_vaccination);
            else if(strcmp(buf,"PROBA_REBRANCHEMENT") == 0)    fscanf(fd,"%lf",&param->proba_rebranchement);
            else if (strcmp(buf,"REBRANCHEMENT") == 0)         fscanf(fd,"%d",&param->nb_rebranchement);
            else if(strcmp(buf,"BARABASI_SOMMET") == 0)        fscanf(fd,"%d",&param->sommet_barabasi);
            else if(strcmp(buf,"TOPOLOGIE") == 0){
                fscanf(fd,"%d",&val);
                param -> topologie = val;
            }

        }
    }
    fclose(fd);
    return param;
}
