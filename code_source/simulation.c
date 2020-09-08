#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <math.h>
#include "simulation.h"
#include "plague.h"
#include "listechaine.h"
#include "stanford.h"
#include "erdosrenyi.h"
#include "anneau.h"
#include "clique.h"
#include "prairie.h"
#include "watts.h"
#include "barabasi.h"


/**
 * Determiner le nombre moyen de voisin de la simulation
 * @param param
 * @param individu
 * @param val
 * @return le nombre moyen de voisin
 */
double determiner_nb_moyen_voisin(struct param_globaux *param,struct individu *individu){
    int calcul = 0;
    for(int i = 0; i < param->taille; i++)
        calcul += individu[i].nb_voisins;
    return (double)calcul/param->taille;
}


/**
 * Met à jour l'état des individus (appelé à la fin du tour pas pendant)
 * @param pc
 * @param individu
 * @param mod 0 : cas des individus devenus infectés
 *            1 : cas des individus devenus résistant
 *            3 : cas des individus qui sont morts
 */
void update_etat(struct cell *pc,struct individu *individu,int mod){
    while(pc != NULL){
        if(mod == 0){
            if(individu[pc->clef].etat != INF)
                individu[pc->clef].etat = INF;
        } else if(mod == 1){
            if(individu[pc->clef].etat != RES)
                individu[pc->clef].etat = RES;
        } else{
            if(individu[pc->clef].etat != MORT)
                individu[pc->clef].etat = MORT;
            if(pc->cpt > 10)
                return;
        }
        pc->cpt += 1;
        pc = pc->suiv;
    }
    return;
}


/**
 * Initialisation des voisins selon la topologie
 * @param individu
 * @param param
 * @param lStanford
 */
void initialiser_voisin(struct individu *individu, struct param_globaux *param,struct cell *lStanford){
    if(param -> topologie == CLIQUE)
        initialiser_voisin_clique(individu,param);
    else if(param -> topologie == ANNEAU)
        initialiser_voisin_anneau(individu,param);
    else if(param -> topologie == ERDOS_RENYI)
        initialiser_voisin_erdosrenyi(individu,param);
    else if(param -> topologie == STANFORD)
        initialiser_voisin_stanford(individu,param,lStanford);
    else if(param -> topologie == PRAIRIE)
        initialiser_voisin_prairie(individu,param);
    else if(param -> topologie == WATTS)
        initialiser_voisin_watts(individu,param);
    return;
}


/**
 * Rechercher le nombre de voisins infectés d'un individu
 * @param individu
 * @param num_individu
 * @param pc
 * @param etat
 * @return le nombre de voisin infecté
 */
int rechercher_nb_voisin_inf(struct individu *individu, int num_individu, struct cell *pc,type_etat etat){
    int cpt = 0;
    while(pc != NULL){
        if(rechercher_cell_liste(individu[num_individu].voisins,pc->clef) == 1 && individu[pc->clef].etat == etat)
            cpt++;
        pc = pc->suiv;
    }
    return cpt;
}


/**
 * Affiche des données sur la simulation à un temps donné
 * @param individu
 * @param param
 * @param lInfecte
 * @param lResistant
 * @param lMort
 * @param lStanford
 * @param jour_sans_infecte
 * @param nb_tour
 */
void affichage_simulation(struct individu *individu,struct param_globaux *param,struct cell *lInfecte,struct cell *lResistant, struct cell *lMort,struct cell *lStanford,int jour_sans_infecte, int nb_tour){
    int nb_inf = 0, nb_res = 0, nb_sain = 0, nb_mort = 0;
    nb_inf = rechercher_nb_individu_cell(lInfecte);
    nb_res = rechercher_nb_individu_cell(lResistant);
    nb_mort = rechercher_nb_individu_cell(lMort);
    nb_sain = param->taille - (nb_inf + nb_res + nb_mort);

    printf("\n\nTour %d\n", nb_tour);
    printf("| POP %d | SAIN %d | INF %d | RES %d | MORT %d |",param->taille,nb_sain,nb_inf,nb_res,nb_mort);
    if(param->taille <= 70){
        printf("\n\nINFECTE : "); afficher_cell_liste(lInfecte,lStanford);
        printf("RESISTANT : "); afficher_cell_liste(lResistant,lStanford);
        printf("MORT : "); afficher_cell_liste(lMort,lStanford);
    }
    ecrire_file(individu,param,nb_tour,nb_inf,nb_res,nb_mort,nb_sain,lInfecte,lResistant,lMort,lStanford);

    if(jour_sans_infecte == 10)
        printf("\nL'agent pathogène ne peut plus se transmettre : plus d'organismes infectés et les victimes ne sont plus infectueux");
    return;
}


/**
 * Écrit dans un fichier texte l'état de chaque individu à chaque tour
 * @param individu
 * @param param
 * @param nb_tour
 * @param nb_inf
 * @param nb_res
 * @param nb_mort
 * @param nb_sain
 * @param lInfecte
 * @param lResistant
 * @param lMort
 * @param lStanford
 */
void ecrire_file(struct individu *individu,struct param_globaux *param,int nb_tour,int nb_inf,int nb_res,int nb_mort,int nb_sain,struct cell *lInfecte,struct cell *lResistant, struct cell *lMort,struct cell *lStanford){
    static const char * type_etat[] = {"S","I","R","M"};
    if(strcmp(param->file_output,"/dev/null") == 0)
        return;

    FILE *fd = write_file(param->file_output,param,1);

    fprintf(fd,"Tour %d\n",nb_tour);
    fprintf(fd,"| POP %d | SAIN %d | INF %d | RES %d | MORT %d |\n\n",param->taille,nb_sain,nb_inf,nb_res,nb_mort);
    for(int i = 0; i < param->taille; i++){
        fprintf(fd,"%d%s ",individu[i].num,type_etat[individu[i].etat]);
    }
    fprintf(fd,"\n\nInfecte : ");ecrire_cell_liste(fd,lInfecte,lStanford);
    fprintf(fd,"Resistant : ");ecrire_cell_liste(fd,lResistant,lStanford);
    fprintf(fd,"Mort : ");ecrire_cell_liste(fd,lMort,lStanford);
    fprintf(fd,"\n\n");
    fclose(fd);
    return;
}


/**
 * Calcul les probabilités de passer d'un état à un autre
 * @param proba
 * @param nb_individu
 * @param val
 * @param mod
 * @return 0 l'individu reste dans son état actuelle
 *         1 l'individu change d'état
 */
int transition_etat(double proba, int nb_individu, int val, int mod){
    double random = fmod(rand(),nb_individu * 100) / (nb_individu);
    double probabilite;
    if(mod == 0) // cas INF
        probabilite = (1-pow((1-proba),val))*100; // calcul de la probabilité de devenir C (S->I)
    else if(mod == 1) // cas RES / SAIN
        probabilite = ((proba*((double)val/10))+(proba/2))*100; // calcul de la probabilité de devenir R (I->R) ou S (R->S)
    else{ // cas MORT
        if((double)val/10 <= 0.9)
            probabilite = (-pow(((double)val/10 - 0.5),2)+proba)*100; // calcul de la probabilité de mourir (I->M)
        else
            probabilite = 0;
    }
    if(random >= probabilite)
        return 0;
    return 1;
}


/**
 * Si un infecté devient résistant met à jour la liste lResistant
 * @param param
 * @param lInfecte
 * @param lResistant
 * @param num_individu
 * @return lResistant
 */
struct cell *transition_res(struct param_globaux *param,struct cell *lInfecte,struct cell *lResistant,int num_individu){
    if(transition_etat(param->proba_r,param->taille,rechercher_cell_compteur(lInfecte,num_individu),1) == 1)
        lResistant = ajout_cell_debut(num_individu,lResistant);
    return lResistant;
}


/**
 * Si un resistant devient sain met à jour la liste lResistant et
 * l'état de l'individu
 * @param individu
 * @param param
 * @param lResistant
 * @param num_individu
 * @return lResistant
 */
struct cell *transition_sain(struct individu *individu,struct param_globaux *param,struct cell *lResistant,int num_individu){
    if(transition_etat(param->proba_s,param->taille,rechercher_cell_compteur(lResistant,num_individu),1) == 1){
        individu[num_individu].etat = SAIN;
        lResistant = suppression_cell_liste(lResistant,NULL,num_individu);
        if(individu[num_individu].vaccin == IMUN) individu[num_individu].vaccin = NV;
    }
    return lResistant;
}


/**
 * Si un individu sain devient infecté met à jour la liste lInfecte 
 * @param individu
 * @param param
 * @param lInfecte
 * @param lMort
 * @param num_individu
 * @return lInfecte
 */
struct cell *transition_infecte(struct individu *individu,struct param_globaux *param,struct cell *lInfecte,struct cell *lMort,int num_individu){
    int val = 0;
    val = rechercher_nb_voisin_inf(individu,num_individu,lInfecte,INF);
    val += rechercher_nb_voisin_inf(individu,num_individu,lMort,MORT);
    if(val > 0)
        if(transition_etat(param->proba_c,param->taille,val,0) == 1)
            lInfecte = ajout_cell_debut(num_individu, lInfecte);
    return lInfecte;
}


/**
 * Si un individu infecté meurt met à jour la liste lMort
 * @param individu
 * @param param
 * @param lInfecte
 * @param lMort
 * @param num_individu
 * @return lMort
 */
struct cell *transition_mort(struct individu *individu,struct param_globaux *param,struct cell *lInfecte,struct cell *lMort,int num_individu){
    if(transition_etat(param->proba_m,param->taille,rechercher_cell_compteur(lInfecte,num_individu),2) == 1){
        lMort = ajout_cell_debut(num_individu,lMort);
        if(param->topologie == PRAIRIE && param->augmenter_rayon != 0.0){
            if(param->augmenter_rayon > 0.0)
                individu[num_individu].rayon_prairie *= param->augmenter_rayon;
            mort_maj_voisin(individu,param,num_individu);
        }
    }
    return lMort;
}


/**
 * Si un individu I ou S devient imunisé met à jour la liste lResistant,
 * son état et sa vaccination
 * @param individu
 * @param param
 * @param lResistant
 * @param num_individu
 * @return lResistant
 */
struct cell *transition_imun(struct individu *individu,struct param_globaux *param,struct cell *lResistant,int num_individu){
    if(individu[num_individu].etat == INF){
        if(transition_etat(param->proba_v/3,param->taille,1,1) == 1){
            individu[num_individu].etat = RES;
            individu[num_individu].vaccin = IMUN;
            lResistant = ajout_cell_debut(num_individu,lResistant);
        }
    }
    else if(individu[num_individu].etat == SAIN){
        if(transition_etat(param->proba_v,param->taille,5,1) == 1){
            individu[num_individu].etat = RES;
            individu[num_individu].vaccin = IMUN;
            lResistant = ajout_cell_debut(num_individu,lResistant);
        }
    }
    return lResistant;
}


/**
 * Simulation de la propagation de l'épidémie
 * Les sains peuvent devenir contaminés
 * Les contaminés peuvent devenir resistants
 * S'ils ne deviennent pas resistant ils peuvent mourir
 * Les résistants peuvent devenir sains
 * La simulation s'arrête quand le nombre de tour maximum est atteint
 * Ou lorsque les contaminés ou les morts ne peuvent plus transmettre l'épidémie
 * @param param
 * @param individu
 * @param lInfecte
 * @param lResistant
 * @param lMort
 * @param lStanford
 */
void simulation(struct param_globaux *param,struct individu *individu,struct cell *lInfecte,struct cell *lResistant, struct cell *lMort,struct cell *lStanford){
    int cpt = 0, jour_sans_infecte = 0;

    while(cpt < param->nb_tour && jour_sans_infecte <= 9){ // les morts peuvent transmettent la maladie pendant 10 jours maximum 
        for(int i = 0; i < param -> taille; i++){
            if(individu[i].etat == SAIN)
                lInfecte = transition_infecte(individu,param,lInfecte,lMort,i);
            else if(individu[i].etat == INF){
                lResistant = transition_res(param,lInfecte,lResistant,i);
                if(rechercher_cell_liste(lResistant,i) == 1){
                    lInfecte = suppression_cell_liste(lInfecte,NULL,i);
                    continue;
                }
                lMort = transition_mort(individu,param,lInfecte,lMort,i);
                if(rechercher_cell_liste(lMort,i) == 1)
                    lInfecte = suppression_cell_liste(lInfecte,NULL,i);
            }
            else if(individu[i].etat == RES)
                lResistant = transition_sain(individu,param,lResistant,i);
        }
        cpt++;

        update_etat(lInfecte,individu,0);
        update_etat(lResistant,individu,1);
        update_etat(lMort,individu,2);

        if(param->jour_vaccination > 0 && cpt == param->jour_vaccination){
            for(int i = 0; i < param->taille; i++){
                lResistant = transition_imun(individu,param,lResistant,i);
                if(rechercher_cell_liste(lResistant,i) == 1 &&
                   rechercher_cell_liste(lInfecte,i) == 1)
                    lInfecte = suppression_cell_liste(lInfecte,NULL,i);
            }
        }

        if(lInfecte == NULL)
            jour_sans_infecte++;
        else
            jour_sans_infecte = 0;

        affichage_simulation(individu,param,lInfecte,lResistant,lMort,lStanford,jour_sans_infecte,cpt);
        param->vitesse_propagation += (rechercher_nb_individu_cell(lInfecte)- param->vitesse_propagation);
    }
    printf("\n## FIN DE SIMULATION ##\n");
    liberer_cell(lInfecte);
    liberer_cell(lResistant);
    liberer_cell(lMort);
    return;
}


/**
 * Initialise la simulation
 * Le(s) patient(s) zéro(s) sont tirés au sort
 * Le(s) individu(s) initialement résistant(s) sont tirés au sort
 * @parami param
 * @param individu
 */
void initialiser_simulation(struct param_globaux *param, struct individu *individu,struct cell *lStanford){
    struct cell * lInfecte = NULL, *lResistant = NULL, *lMort = NULL;
    FILE *fd = write_file(param->file_output,param,1);

    lInfecte = initialiser_cell_debut(lInfecte,individu,param,0);
    lResistant = initialiser_cell_debut(lResistant,individu,param,1);

    if(lInfecte != NULL){
        printf("Il y a %d patient(s) zero() et ce sont les individus : ",param->nb_patient_zero);
        afficher_cell_liste(lInfecte,lStanford);
        fprintf(fd,"Il y a %d patient(s) zero() et ce sont les individus : ",param->nb_patient_zero);
        ecrire_cell_liste(fd,lInfecte,lStanford);
    }
    if(lResistant != NULL){
        printf("Il y a  %d individus resistant : ",param->nb_resistant_debut);
        afficher_cell_liste(lResistant,lStanford);
        fprintf(fd,"Il y a  %d individus resistant : ",param->nb_resistant_debut);
        ecrire_cell_liste(fd,lResistant,lStanford);
    }
    fclose(fd);
    printf("\n## DÉBUT DE SIMULATION ##");
    param->vitesse_propagation = param->nb_patient_zero;

    simulation(param,individu,lInfecte,lResistant,lMort,lStanford);
    return;
}
