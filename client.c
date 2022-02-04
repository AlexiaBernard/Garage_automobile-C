#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include "types.h"

int main(int argc, char const *argv[]){
    /*------------Vérification des arguments---------------*/

    if (argc < 2)
    {
        fprintf(stderr, "Attention, un client a besoin de 2 arguments : nb_chefs cles_chefs\n");
        exit(1);
    }

    int nb_chefs = (int)strtol(argv[1], NULL, 0);
    key_t clefs_chefs = (int)strtol(argv[2], NULL, 0);

/*
    couleur(ROUGE);
    fprintf(stdout, "Un client vient d'arriver au garage.\n");
    couleur(REINIT);*/

    //Signal entre client et chef ? 
}