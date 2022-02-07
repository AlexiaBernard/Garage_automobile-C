#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <assert.h>
#include <time.h>
#include <signal.h> 
#include "types.h"

int file_mess_ch_mecanicien;
int semid;
int sig=1;

//DANS MECANICIEN CE QUIL MANQUE : 
//Arrêt quand reception du SIGNINT
//Verifier que le travail se termine avant de s'arrêter à la recpetion du signal

void action(){
    printf("Contrôle c mecanicien\n");
    sig=2;
}

int main(int argc, char const *argv[]){
    int numero_ordre;
    key_t cle_ch_mecanicien;
    ssize_t nb_lus, nb_envoi;
    requete_chef requete;
    reponse_mecanicien reponse;
    int nb_1, nb_2, nb_3, nb_4;

    sig_t s1 = signal(SIGINT,action);

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
    cle_ch_mecanicien= ftok(FICHIER_CLE,'a');
    assert(cle_ch_mecanicien!= -1);

        /*----------Récupération----------*/
    file_mess_ch_mecanicien = msgget(cle_ch_mecanicien,0);
    assert(file_mess_ch_mecanicien != -1);

    /*--------------Sémarphores--------------*/
    semid = semget(cle_ch_mecanicien,4,0);
	assert(semid >= 0);

    /*------Récupération des messages---------*/

    while(sig==1){
        /* mecanicien attend des requetes, de type numero_ordre :        */
        nb_lus = msgrcv(file_mess_ch_mecanicien,(void *) &requete, TAILLE_REQUETE_CHEF, 1, 0);

        assert(nb_lus != -1);

        couleur(BLEU);
        fprintf(stdout, "Le mécanicien n°%d vient de recevoir une requête du chef %d.\n", numero_ordre, requete.chef);
        //fprintf(stdout,"coucou 1:%d 2:%d 3:%d 4:%d\n",requete.nb_1,requete.nb_2,requete.nb_3,requete.nb_4);
        fprintf(stdout,"\t Durée du travail : %d.\n", requete.duree);
        couleur(REINIT);


        /* Réservation des outils */

        nb_1 = requete.nb_1;
        nb_2 = requete.nb_2;
        nb_3 = requete.nb_3;
        nb_4 = requete.nb_4;

        struct sembuf p[] = { {0,-nb_1,0}, {1,-nb_2,0}, {2,-nb_3,0}, {3,-nb_4,0} };
        struct sembuf v[] = { {0,+nb_1,0}, {1,+nb_2,0}, {2,+nb_3,0}, {3,+nb_4,0} };

        /* Verrou des outils */
        assert(semop(semid,p,4) != -1);

        /* traitement de la requête */
        couleur(BLEU);
        fprintf(stdout, "Le mécanicien n°%d commence à travailler : durée du travail %d.\n",numero_ordre, requete.duree);
        couleur(REINIT);

        usleep(requete.duree);

        couleur(BLEU);
        fprintf(stdout, "Le mécanicien n°%d a fini de travailler.\n",numero_ordre);
        couleur(REINIT);

        /* Libération des outils */
        semop(semid,v,4);
        
        srand(time(NULL));
        reponse.type = requete.chef;
        reponse.resultat = rand()%100;
        reponse.client = requete.client;


        /* Envoi de la réponse */
        couleur(BLEU);
        fprintf(stdout, "Le mécanicien n°%d envoie le résultat de son travail au chef n°%d.\n",numero_ordre, requete.chef);
        fprintf(stdout, "\t Résultat : %d.\n",reponse.resultat);
        couleur(REINIT);
        nb_envoi = msgsnd(file_mess_ch_mecanicien, &reponse, TAILLE_REPONSE_MECANICIEN, 2);

        assert(nb_envoi != -1);

    }
    exit(0);   
}