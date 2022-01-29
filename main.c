#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

int main(int argc, char const *argv[]){

    /*------------Vérification des arguments---------------*/

    if (argc<6){
        fprintf(stderr, "Attention, il faut 6 arguments : nb_chefs nb_mecaniciens nb_1 nb_2 nb_3 nb_4\n");
        exit(1);
    }

    int nb_chefs = strtol(argv[1]);
    int nb_mecaniciens = strtol(argv[2]);
    int nb_1 = strtol(argv[3]);
    int nb_2 = strtol(argv[4]);
    int nb_3 = strtol(argv[5]);
    int nb_4 = strtol(argv[6]);

    if(nb_chefs<2){
        fprintf(stderr, "Attention, il faut au moins deux chefs d'ateliers\n");
        exit(1);
    }

    if (nb_mecaniciens<3){
        fprintf(stderr, "Attention, il faut au moins trois mécaniciens\n");
        exit(1);
    }

    /*-----------Création des IPC nécessaire--------------*/
    key_t cles_chef; //en faire un tableau pour en mettre plusieurs ? 

    /*------------Lancer les chefs d'ateliers-------------*/

    for(int i=0; i<nb_chefs; i++){
        //numéro d'ordre = i ?
        chef_atelier(i, nb_1, nb_2, nb_3, nb_4);
    }

    /*--------------Lancer les mécaniciens---------------*/
    for(int i=0; i<nb_mecaniciens; i++){
        //numéro d'ordre = i ?
        mecanicien(i);
    }

    /*--------------Lancer les mécaniciens---------------*/
    int inconnu; //on en lance combien ? :/
    for(int i=0; i<inconnu; i++){
        mecanicien(nb_chefs, cles_chef);
    }


    
    exit(0);
}
