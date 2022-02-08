#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <signal.h>
#include <assert.h>
#include <time.h>
#include "types.h"

int file_mess_ch_mecanicien;
int file_mess_clients; 
int semid_outils;
int semid_clients;

void arret(){
    /* Arret du serveur en detruisant la file de message */
    assert(msgctl(file_mess_ch_mecanicien, IPC_RMID, NULL) >= 0);
    assert(msgctl(file_mess_clients, IPC_RMID, NULL) >= 0);
    assert(semctl(semid_outils,0,IPC_RMID) >= 0);
    assert(semctl(semid_clients,0,IPC_RMID) >= 0);
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
    key_t cle_client;
    key_t cle_ch_mecanicien;

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

        /*------Calcul des clés-------*/

    cle_ch_mecanicien = ftok(FICHIER_CLE,'a');
    assert(cle_ch_mecanicien != -1);

    cle_client = ftok(FICHIER_CLE,'b');
    assert(cle_client != -1);

        /*-----------La file de message--------------*/

            /*------Création-------*/

    file_mess_ch_mecanicien = msgget(cle_ch_mecanicien, IPC_CREAT|0666);
    assert(file_mess_ch_mecanicien != -1);

    file_mess_clients = msgget(cle_client, IPC_CREAT|0666);
    assert(file_mess_clients != -1);

        /*-----------Sémaphores--------------*/

                /*------Création-------*/

    semid_outils = semget(cle_ch_mecanicien, 4 ,IPC_CREAT|0666);
    //pour le moment j'ai mis 4 semaphores pour le nb d'outils
    assert(semid_outils != -1);

    semid_clients = semget(cle_client, nb_chefs ,IPC_CREAT|0666);
    //pour le moment j'ai mis nb_chefs semaphores
    assert(semid_clients != -1);

                /*------Initialisation-------*/
    //ici initialisation d'un semaphore pour les outils
    assert(semctl(semid_outils,0,SETVAL,nb_1) >=0); //initialise à "nb_1" qui est le nombre d outil de cette categorie
    assert(semctl(semid_outils,1,SETVAL,nb_2) >=0);
    assert(semctl(semid_outils,2,SETVAL,nb_3) >=0);
    assert(semctl(semid_outils,3,SETVAL,nb_4) >=0);

    //ici initialisation d'un semaphore pour les clients
    unsigned short *tab;
    tab = (unsigned short *) malloc(nb_chefs*sizeof(unsigned short));
    for(int i=0; i<nb_chefs; i++){
        tab[i] = 0;
    }

    assert(semctl(semid_clients,0,SETALL,tab) >=0); //Initialise à 0 comme ca on pourra avoir le nb en attente

    //Pas sûre qu'il faut le mettre ici (j'ai pris exemple d'un truc du prof)
    assert(set_signal_handler(SIGINT,arret) == 0);

    /*------------Lancer les chefs d'ateliers-------------*/
    char argv2[2];
    char argv3[2];
    char argv4[2];
    char argv5[2];

    snprintf(argv2, sizeof(argv2), "%d", nb_1);
    snprintf(argv3, sizeof(argv3), "%d", nb_2);
    snprintf(argv4, sizeof(argv4), "%d", nb_3);
    snprintf(argv5, sizeof(argv5), "%d", nb_4);


    for(int i=2; i<nb_chefs+2; i++){
        
        char argv1[2];
        snprintf(argv1, sizeof(argv1), "%d", i);
        

        p = fork();
        if(p==0){ //fils
			execl("./chef_atelier", "./chef_atelier", argv1, argv2, argv3, argv4, argv5, NULL);
            break;
        }
    }

    sleep(1);

    /*--------------Lancer les mécaniciens---------------*/
    for(int j=1; j<=nb_mecaniciens; j++){
        char argv1[2];
        snprintf(argv1, sizeof(argv1), "%d", j);
        p = fork();
        if(p==0){ //fils
            execl("./mecanicien", "./mecanicien",argv1,NULL);
            break;
        }
    }

    sleep(1);

    /*--------------Lancer les clients---------------*/
    
    //ATTENTION les clients ont besoin uniquement de la clé contenu dans cle_client
    //Ici il faut mettre un temporisation pour les creer
    
    int duree;
    srand(time(NULL));
    int inconnu = 2; //pour l'instant on choisi et on pourra mettre un nombre aléatoire
    char a1[2];
    snprintf(a1, sizeof(a1), "%d", nb_chefs);
    char a2[200];
    snprintf(a2, sizeof(a2), "%d", cle_client);

    for(int i=0; i<inconnu; i++){
        p = fork();
        if(p==0){ //fils
            execl("./client", "./client", a1, a2, NULL);
            break;
        }
        duree = rand()%10;
        sleep(duree);
    }
    arret();
    exit(0);
}
