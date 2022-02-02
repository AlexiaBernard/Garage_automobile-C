#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int main(int argc, char const *argv[]){
    /*------------Vérification des arguments---------------*/

    if (argc<5){
        fprintf(stderr, "Attention, un chef d'atelier a besoin de 5 arguments : numero_ordre nb_1 nb_2 nb_3 nb_4\n");
        exit(1);
    }

    int numero_ordre = (int)strtol(argv[1], NULL, 0);
    int nb_1 = (int)strtol(argv[2], NULL, 0);
    int nb_2 = (int)strtol(argv[3], NULL, 0);
    int nb_3 = (int)strtol(argv[4], NULL, 0);
    int nb_4 = (int)strtol(argv[5], NULL, 0);
}