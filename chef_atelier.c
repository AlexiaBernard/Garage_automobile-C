#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <assert.h>
#include "types.h"

int file_mess;
int semid;

int main(int argc, char const *argv[]){
    int numero_ordre;
    int nb_1, nb_2, nb_3, nb_4;
    key_t cle ;
    int mecanicien;
    ssize_t nb_lus, nb_envoi;
    requete_client requete;
    reponse_chef reponse;

    /*------------Vérification des arguments---------------*/
    if (argc<5){
        fprintf(stderr, "Attention, un chef d'atelier a besoin de 5 arguments : numero_ordre nb_1 nb_2 nb_3 nb_4 = %d\n", argc);
        exit(1);
    }

    numero_ordre = (int)strtol(argv[1], NULL, 0);
    nb_1 = (int)strtol(argv[2], NULL, 0);
    nb_2 = (int)strtol(argv[3], NULL, 0);
    nb_3 = (int)strtol(argv[4], NULL, 0);
    nb_4 = (int)strtol(argv[5], NULL, 0);

    couleur(JAUNE);
    fprintf(stdout,"Le chef d'atelier n°%d vient d'arriver au garage.\n", numero_ordre);
    couleur(REINIT);

    //Signal entre client et chef ? 

}

    