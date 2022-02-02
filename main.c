#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <assert.h>
#include "types.h"

int file_mess; /* ID de la file, necessairement global pour pouvoir la supprimer a la terminaison */
//Peut être en faire un tableau s'il en faut plusieurs

void arret(int s){
    /* Arret du serveur en detruisant la file de message */
    assert(msgctl(file_mess, IPC_RMID, NULL) >= 0);
}

//Fonction du prof pour arreter quand recoit message SIGINT
int set_signal_handler(int signo, void (*handler)(int)) {
	struct sigaction sa;
	sa.sa_handler = handler;    // call `handler` on signal
	sigemptyset(&sa.sa_mask);   // don't block other signals in handler
	sa.sa_flags = 0 ;            //  don't restart system calls
	return sigaction(signo, &sa, NULL);
}

int main(int argc, char const *argv[]){

    pid_t p ;

    /*------------Vérification des arguments---------------*/

    if (argc<6){
        fprintf(stderr, "Attention, il faut 6 arguments : nb_chefs nb_mecaniciens nb_1 nb_2 nb_3 nb_4\n");
        exit(-1);
    }

    int nb_chefs = (int)strtol(argv[1], NULL, 0);
    int nb_mecaniciens = (int)strtol(argv[2], NULL, 0);
    int nb_1 = (int)strtol(argv[3], NULL, 0);
    int nb_2 = (int)strtol(argv[4], NULL, 0);
    int nb_3 = (int)strtol(argv[5], NULL, 0);
    int nb_4 = (int)strtol(argv[6], NULL, 0);

    if(nb_chefs<2){
        fprintf(stderr, "Attention, il faut au moins deux chefs d'ateliers\n");
        exit(-1);
    }

    if (nb_mecaniciens<3){
        fprintf(stderr, "Attention, il faut au moins trois mécaniciens\n");
        exit(-1);
    }

    /*-----------Création des IPC nécessaire--------------*/

    key_t cles_chefs; //en faire un tableau pour en mettre plusieurs ?
    //Pour le moment j'en ai mis que une pour la file de message

        /*-----------La file de message--------------*/

            /*------Calcul de la cle-------*/
    cles_chefs = ftok(FICHIER_CLE,'a');

    assert(cles_chefs != -1);

            /*------Création-------*/

    file_mess = msgget(cles_chefs, IPC_CREAT|0600);

    assert(file_mess != -1);

    //Pas sûre qu'il faut le mettre ici (j'ai prit exemple d'un truc du prof)
    assert(set_signal_handler(SIGINT,arret) == 0);

    /*------------Lancer les chefs d'ateliers-------------*/

    for(int i=0; i<nb_chefs; i++){
        p = fork();
        if(p==0){ //fils
			execl("./chef_atelier","./chef_atelier",i,nb_1,nb_2,nb_3,nb_4,NULL);
            break;
        }
    }

    /*--------------Lancer les mécaniciens---------------*/
    for(int i=0; i<nb_mecaniciens; i++){
        p = fork();
        if(p==0){ //fils
            execl("./mecanicien", "./mecanicien",i,NULL);
            break;
        }
    }

    /*--------------Lancer les clients---------------*/
    int inconnu = 2; //on en lance combien ? :/
    //Ici il faut mettre un temporisation pour les creer
    for(int i=0; i<inconnu; i++){
        execl("./client","./client",nb_chefs,cles_chefs,NULL);
    }
    
    exit(0);
}


/*
Chef d'atelier envoie au mecanicien les messages par une file de mesage e y mettant son id -> il y aura une unique file de message
Une fois que le mecanicien a fini il doit envoyer un message au chef d'atelier grâce à l'id du chef d'atelier
*/