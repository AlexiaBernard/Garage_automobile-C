#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){

    if (argc<6){
        fprintf(stderr, "Attention, il faut 6 arguments : nb_chefs nb_mecaniciens nb_1 nb_2 nb_3 nb_4\n");
        exit(1);
    }

    if(argv[1]<2){
        fprintf(stderr, "Attention, il faut au moins deux chefs d'ateliers\n");
        exit(1);
    }

    if (argv[2]<3){
        fprintf(stderr, "Attention, il faut au moins trois mécaniciens\n");
        exit(1);
    }
    

    
    exit(0);
}
