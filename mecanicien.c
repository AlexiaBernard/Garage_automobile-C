#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <assert.h>
#include "types.h"

int file_mess;

int main(int argc, char const *argv[]){

    key_t cle ;
    int numero_ordre;
    ssize_t nb_lus;
    requete_chef requete;

    /*------------Vérification des arguments---------------*/

    if (argc<1){
        fprintf(stderr, "Attention, un mécanicien a besoin de 1 argument : numero_ordre\n");
        exit(-1);
    }

    numero_ordre = (int)strtol(argv[1],NULL,0);

    /*--------------File de message--------------*/

        /*----------Calcul de la cle----------*/
    cle = ftok(FICHIER_CLE,'a');
    assert(cle != -1);
    
        /*----------Récupération----------*/
    file_mess = msgget(cle,0);
    assert(file_mess != -1);


    /*------Récupération des messages---------*/

    while(1){
        /* mecanicien attend des requetes, de type numero_ordre :        */
        nb_lus = msgrcv(file_mess,(void *) &requete, TAILLE_REQUETE_CHEF, numero_ordre, 0);
    }

    
}