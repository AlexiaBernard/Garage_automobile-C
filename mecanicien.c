#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

int main(int argc, char const *argv[]){
    /*------------Vérification des arguments---------------*/

    if (argc<1){
        fprintf(stderr, "Attention, un mécanicien a besoin de 1 argument : numero_ordre\n");
        exit(1);
    }

    int numero_ordre = strtol(argv[1], &argv[2], 10);
}