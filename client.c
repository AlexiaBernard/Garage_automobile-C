#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

int main(int argc, char const *argv[]){
    /*------------Vérification des arguments---------------*/

    if (argc < 2)
    {
        fprintf(stderr, "Attention, un client a besoin de 2 arguments : nb_chefs cles_chefs\n");
        exit(1);
    }

    int nb_chefs = strtol(argv[1], &argv[2], 10);
    key_t clefs_chefs = strtol(argv[2], &argv[3], 10);
}