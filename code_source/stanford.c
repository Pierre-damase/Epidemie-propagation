#include <stdlib.h>
#include <stdio.h>
#include "stanford.h"
#include "plague.h"
#include "listechaine.h"

#define SIZE 64


/**
 * Deuxième passe pour initialiser les voisins à partir d'un
 * fichier texte au format de Stanford
 * Conversion du num réel de l'individu dans le fichier au num
 * de ce dernier dans le tableau d'individu
 * @param individu
 * @param param
 * @param lStanford
 */
void initialiser_voisin_stanford(struct individu *individu, struct param_globaux *param,struct cell *lStanford){
    int c, origine, extremite;
    FILE *fd = open_file(param->stanford_file,param);
    while(1){
        c = fgetc(fd);
        if(c == EOF)
            break;
        else if(c == '#'){
            do{
                c = fgetc(fd);
            } while(c != '\n');
        }
        else if(c != '\n'){
            fseek(fd,-1,SEEK_CUR);
            fscanf(fd,"%d",&origine);
            fscanf(fd,"%d",&extremite);

            origine = rechercher_cell_stanford(lStanford,origine);
            extremite = rechercher_cell_stanford(lStanford,extremite);

            if(extremite == -1 || origine == -1) continue;

            else if(rechercher_cell_liste(individu[extremite].voisins,origine) != 1){
                    individu[origine].voisins = ajout_cell_debut(extremite, individu[origine].voisins);
                    individu[extremite].voisins = ajout_cell_debut(origine, individu[extremite].voisins);
                }
        }
    }
    fclose(fd);
    for(int i = 0; i < param->taille; i++)
        individu[i].nb_voisins = rechercher_nb_individu_cell(individu[i].voisins);
    //afficher_cell_voisins(individu,param->taille);
    return;
}


/**
 * Première passe pour déterminer les paramètres du graphe lu :
 * nmax, n(nombre effectif de sommet), (nombre d'arête)
 * Nom du fichier lu passé en paramètre dans le fichier de configuration
 * @param param
 * @param lStanford : liste des individus du graphe
 * stocke dans le champ clef le num de l'individu dans le tab d'individu
 * stocke dans le champ cpt le num réel de l'individu dans le graphe
 * @return lStanford
 */
struct cell *determiner_parametre_stanford(struct param_globaux *param, struct cell *lStanford){
    int c, origine = 0, nb_arrete = 0, tmp = 0, individu = 0;
    FILE *fd = open_file(param->stanford_file,param);
    while(1){
        c = fgetc(fd);
        if(c == EOF)
            break;
        else if(c == '#'){
            do{
                c = fgetc(fd);
            } while(c != '\n');
        }
        else if(c != '\n'){
            fseek(fd,-1,SEEK_CUR);
            fscanf(fd,"%d",&origine);
            nb_arrete++;
            if(origine > tmp) tmp = origine;
            if(rechercher_cell_stanford(lStanford,origine) == -1){
                lStanford = ajout_cell_debut(individu,lStanford);
                lStanford -> cpt = origine; // ici cpt correspond à la valeur réel de l'individu dans le fichier
                individu++;
            }
        }
    }
    fclose(fd);
    param->taille = individu;
    param->stanford_arrete = nb_arrete/2;
    param->stanford_nmax = origine;
    return lStanford;
}


/**
 * Demande le nom du fichier de sortie puis écrit au format de Stanford 
 * @param individu
 * @param param
 */
void ecrire_stanford_format(struct individu *individu,struct param_globaux *param, struct cell *lStanford){
    char buf[SIZE];
    printf("\nNom du fichier de sorti format Stanford : ");
    scanf("%s",buf);

    FILE *fd = write_file(buf,param,0);

    for(int i = 0; i < param->taille; i++){
        for(int j = i+1; j < param->taille; j++){
            if(rechercher_cell_liste(individu[i].voisins,j) == 1){
                if(lStanford != NULL)
                    fprintf(fd,"%d %d\n",rechercher_cell_compteur(lStanford,i),rechercher_cell_compteur(lStanford,j));
                else
                    fprintf(fd,"%d %d\n",i,j);
            }
        }
    }
    fclose(fd);
    printf("\n\n\n\n\n\n\n\n\nRetour à la sélection de la simulation\n\n");
    return;
}
