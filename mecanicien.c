#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <assert.h>
#include "types.h"

int file_mess;
int semid;

//DANS MECANICIEN CE QUIL MANQUE : 
//Arrêt quand reception du SIGNINT
//Verifier que le travail se termine avant de s'arrêter à la recpetion du signal

int main(int argc, char const *argv[]){

    key_t cle ;
    int numero_ordre;
    ssize_t nb_lus, nb_envoi;
    struct requete_chef requete;
    struct reponse_mecanicien reponse;
    //struct sembuf p[];
    //struct sembuf v[];
    int nb_1, nb_2, nb_3, nb_4;

    /*------------Vérification des arguments---------------*/

    if (argc<1){
        fprintf(stderr, "Attention, un mécanicien a besoin de 1 argument : numero_ordre\n");
        exit(-1);
    }

    numero_ordre = (int)strtol(argv[1],NULL,0);

    couleur(BLEU);
    fprintf(stdout, "Le mécanicien n°%d vient d'arriver au garage.\n", numero_ordre);
    couleur(REINIT);

    /*--------------File de message--------------*/

        /*----------Calcul de la cle----------*/
    cle = ftok(FICHIER_CLE,'a');
    assert(cle != -1);
    
        /*----------Récupération----------*/
    file_mess = msgget(cle,0);
    assert(file_mess != -1);

    /*--------------Sémarphores--------------*/
    semid = semget(cle,4,0);
	assert(semid >= 0);

    /*------Récupération des messages---------*/

    while(1){
        /* mecanicien attend des requetes, de type numero_ordre :        */
        nb_lus = msgrcv(file_mess,(void *) &requete, TAILLE_REQUETE_CHEF, numero_ordre, 0);

        assert(nb_lus != -1);

        couleur(BLEU);
        fprintf(stdout, "Le mécanicien n°%d vient de recevoir une requête du chef %d.\n", numero_ordre, requete.chef);
        fprintf(stdout,"\t Durée du travail : %d.\n", requete.duree);
        couleur(REINIT);

        /* Réservation des outils */
        int pnb_1 = -requete.nb_1;
        int pnb_2 = -requete.nb_2;
        int pnb_3 = -requete.nb_3;
        int pnb_4 = -requete.nb_4;
        int vnb_1 = requete.nb_1;
        int vnb_2 = requete.nb_2;
        int vnb_3 = requete.nb_3;
        int vnb_4 = requete.nb_4;

        struct sembuf p[] = { {0,pnb_1,0}, {1,pnb_2,0}, {2,pnb_3,0}, {3,pnb_4,0} };
        struct sembuf v[] = { {0,vnb_1,0}, {1,vnb_2,0}, {2,vnb_3,0}, {3,vnb_4,0} };

        semop(semid,p,4);

        /* traitement de la requête */
        couleur(BLEU);
        fprintf(stdout, "Le mécanicien n°%d commence à travailler : durée du travail %d.\n",numero_ordre, requete.duree);
        couleur(REINIT);

        sleep(requete.duree);

        couleur(BLEU);
        fprintf(stdout, "Le mécanicien n°%d a fini de travailler.\n",numero_ordre);
        couleur(REINIT);

        /* Libération des outils */
        semop(semid,v,4);

        reponse.type = requete.chef;
        reponse.resultat = 10;
        reponse.client = requete.client;


        /* Envoi de la réponse */
        nb_envoi = msgsnd(file_mess, &reponse, TAILLE_REPONSE_MECANICIEN, 0);

        assert(nb_envoi != -1);

    }
    exit(0);   
}