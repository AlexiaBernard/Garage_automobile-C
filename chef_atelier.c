#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <assert.h>
#include <time.h>
#include <signal.h> 
#include "types.h"

int file_mess_client;
int file_mess_ch_mecanicien;
int semid;
int sig=1;

void action(){
	sig=2;
}

int main(int argc, char const *argv[]){
	int numero_ordre;
	int nb_1, nb_2, nb_3, nb_4;
	key_t cle_ch_mecanicien ;
	key_t cle_client ;
	ssize_t nb_lus, nb_envoi;
	requete_client req_client;
	requete_chef req_chef;

	reponse_chef rep_chef;
	reponse_mecanicien rep_mecanicien;

	signal(SIGINT,action);

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

	/*--------------File de message--------------*/

		/*----------Calcul de la cle----------*/
	cle_ch_mecanicien = ftok(FICHIER_CLE,'a');
	assert(cle_ch_mecanicien != -1);

	cle_client = ftok(FICHIER_CLE,'b');
	assert(cle_client != -1);
	
		/*----------Récupération----------*/
	file_mess_ch_mecanicien = msgget(cle_ch_mecanicien,0);
	assert(file_mess_ch_mecanicien != -1);

	file_mess_client = msgget(cle_client,0);
	assert(file_mess_client != -1);

	/*--------------Sémarphores--------------*/
	semid = semget(cle_client,0,0);
	assert(semid >= 0);

	struct sembuf v[] = {{numero_ordre-2,+1,0}};

	/*------Récupération des messages---------*/

	while(1){
		//le chef d'atelier attend des requetes du client, de type numero_ordre
		nb_lus = msgrcv(file_mess_client,(void *) &req_client, TAILLE_REQUETE_CLIENT, numero_ordre, 0);

		if(sig==1){

			assert(nb_lus != -1);

			semop(semid,v,1);

			couleur(JAUNE);
			fprintf(stdout, "Le chef d'atelier n°%d vient de recevoir une requête du client %d.\n", numero_ordre, req_client.client);
			couleur(REINIT);

			/* Traitement de la requête du client*/
			couleur(JAUNE);
			fprintf(stdout, "Le chef d'atelier n°%d prépare le travail des mécaniciens.\n",numero_ordre);
			couleur(REINIT);

			srand(time(NULL));
			req_chef.type = 1;
			req_chef.duree = rand()%30+1;
			req_chef.nb_1 = rand()%nb_1;
			req_chef.nb_2 = rand()%nb_2;
			req_chef.nb_3 = rand()%nb_3;
			req_chef.nb_4 = rand()%nb_4;
			req_chef.chef = numero_ordre;
			req_chef.client = req_client.client;

			if(req_chef.nb_1==0){
				req_chef.nb_1=1;
			}
			if(req_chef.nb_2==0){
				req_chef.nb_2=1;
			}
			if(req_chef.nb_3==0){
				req_chef.nb_3=1;
			}
			if(req_chef.nb_4==0){
				req_chef.nb_4=1;
			}

			/* Envoi du travail au mecanicien */
			couleur(JAUNE);
			fprintf(stdout, "Le chef d'atelier n°%d envoie le travail aux mécaniciens.\n",numero_ordre);
			couleur(REINIT);
			nb_envoi = msgsnd(file_mess_ch_mecanicien, &req_chef, TAILLE_REQUETE_CHEF, 1);
			assert(nb_envoi != -1);

			/* Attente du résultat du travail du mecanicien */
			nb_lus = msgrcv(file_mess_ch_mecanicien,(void *) &rep_mecanicien, TAILLE_REPONSE_MECANICIEN, numero_ordre+1, 0);

			assert(nb_lus != -1);

			couleur(JAUNE);
			fprintf(stdout, "Le chef d'atelier n°%d vient de recevoir la réponse du travail du client %d.\n", numero_ordre, rep_mecanicien.client);

			/* Traitement de la reponse du mecanicien*/

			rep_chef.type = rep_mecanicien.client;
			rep_chef.resultat = rep_mecanicien.resultat;

			/* Envoi du travail au client */
			couleur(JAUNE);
			fprintf(stdout, "Le chef d'atelier n°%d envoi le résultat du travail au client %ld.\n",numero_ordre, rep_chef.type);
			couleur(REINIT);
			
			nb_envoi = msgsnd(file_mess_client, &rep_chef, TAILLE_REPONSE_CHEF, 0);

			assert(nb_envoi != -1);
		} else {
			break;
		}
	}
	exit(0);   
}

	