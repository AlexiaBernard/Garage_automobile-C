#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
    /*------------Vérification des arguments---------------*/

    if (argc<5){
        fprintf(stderr, "Attention, un chef d'atelier a besoin de 5 arguments : numero_ordre nb_1 nb_2 nb_3 nb_4\n");
        exit(1);
    }

    int numero_ordre = strtol(argv[1], &argv[2], 10);
    int nb_1 = strtol(argv[2], &argv[3], 10);
    int nb_2 = strtol(argv[3], &argv[4], 10);
    int nb_3 = strtol(argv[4], &argv[5], 10);
    int nb_4 = strtol(argv[5], &argv[5], 10);
}