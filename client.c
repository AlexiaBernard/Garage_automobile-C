#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <assert.h>
#include "types.h"

int file_mess;
int semid;

int main(int argc, char const *argv[]){

    int nb_envoi,nb_recu,nb_lus;
    requete_client requete;
    reponse_chef reponse;

    /*------------Vérification des arguments---------------*/

    if (argc < 2){
        fprintf(stderr, "Attention, un client a besoin de 2 arguments : nb_chefs cles_chefs\n");
        exit(1);
    }

    int nb_chefs = (int)strtol(argv[1], NULL, 0);
    key_t cle = (int)strtol(argv[2], NULL, 0);

    couleur(ROUGE);
    fprintf(stdout, "Un client vient d'arriver au garage.\n");
    couleur(REINIT);

    /*--------------File de message--------------*/
    
    file_mess = msgget(cle,0);
    assert(file_mess != -1);

    /*--------------Sémarphores--------------*/
    semid = shmget(cle,4,0);
	assert(semid >= 0);

    //se mettre en attente du chef d'atelier le moins occupé (moyen à définir par vous)
    //PROBLEME : Comment faire pour récupérer la "liste d'attente" (=la valeur d'un semaphore)
    int num_chef = 0; //revient au numéro du chef choisi

    //struct sembuf p[] = {{num_chef,-1,0}};
    //struct sembuf v[] = {{num_chef,+1,0}};

    //semop(semid,p,1);

    /* Envoi du travail au chef d'atelier */
    couleur(ROUGE);
    fprintf(stdout, "Le client n°%d envoie du travail au chef d'atelier n°%d\n", getpid(), num_chef);
    couleur(REINIT);

    requete.type = num_chef;
    requete.client = getpid();

    nb_envoi = msgsnd(file_mess, &requete, TAILLE_REQUETE_CLIENT, 0);

    assert(nb_envoi != -1);

    /* Le client attend la réponse du chef */
    nb_lus = msgrcv(file_mess,(void *) &reponse, TAILLE_REPONSE_CHEF, getpid(), 0);
    assert(nb_lus != -1);

    //afficher la requête et son résultat.
    couleur(ROUGE);
    fprintf(stdout, "Le client n°%d a reçu le résultat du travail demandé.\n", getpid());
    fprintf(stdout, "\t Résultat obtenu : %d.\n", reponse.resultat);
    couleur(REINIT);

    //semop(semid,v,1);

    exit(0);

}