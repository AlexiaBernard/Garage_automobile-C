#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h> 
#include "types.h"

int file_mess_clients;
int semid;
int mutex;
int shmid;
int sig = 1;

void action(){
	sig=2;
}

int main(int argc, char const *argv[]){

	int nb_envoi, nb_lus, nb_chefs, *file_attente,i;
	pid_t pid = getpid();
	key_t cle, cle_mutex;
	long int num_chef;
	unsigned short int min;
	unsigned short int val;

	requete_client requete;
	reponse_chef reponse;

	struct sembuf p_m[] = {{0,-1,0}};
	struct sembuf v_m[] = {{0,+1,0}};

	signal(SIGINT,action);

	/*------------Vérification des arguments---------------*/

	if (argc < 2){
		fprintf(stderr, "Attention, un client a besoin de 3 arguments : nb_chefs cle_chefs cle_mutex\n");
		exit(1);
	}

	nb_chefs = (int) strtol(argv[1], NULL, 0);
	cle = (key_t) strtol(argv[2], NULL, 0);
	cle_mutex = (key_t) strtol(argv[3], NULL, 0);

	couleur(BLANC);
	fprintf(stdout, "Le client n°%d vient d'arriver au garage.\n", getpid());
	couleur(REINIT);

	/*----------File de message----------*/
	file_mess_clients = msgget(cle,0);
	assert(file_mess_clients != -1);

	/*--------------Sémarphores--------------*/
	semid = semget(cle,nb_chefs,0);
	assert(semid >= 0);

	mutex = semget(cle_mutex,1,0);
	assert(semid >= 0);

	/*--------------Mémoire partagées--------------*/
	shmid=shmget(cle, nb_chefs*sizeof(int), 0);
	assert(shmid >= 0);

	file_attente = (int*)shmat(shmid,NULL,0);
	assert(file_attente != (void*)-1);


	//Mise en attente du chef d'atelier ayant le moins dans la file d'attente
	semop(mutex,p_m,1);

	for(i=0; i<nb_chefs; i++){
		val = file_attente[i];
		if (i==0){
			min = val;
			num_chef = i;
		} else if(val<min) {
			min = val;
			num_chef = i;
		}
	}

	semop(mutex,v_m,1);

	//Attention ! 0 n'est pas possible !
	num_chef += 2; 

	struct sembuf p[] = {{num_chef-2,-1,0}};

	file_attente[num_chef-2] = +1;
	semop(semid,p,1);
	file_attente[num_chef-2] = -1;

	if(sig==1){
		/* Envoi du travail au chef d'atelier */
		couleur(BLANC);
		fprintf(stdout, "Le client n°%d envoie du travail au chef d'atelier n°%ld\n", pid, num_chef);
		couleur(REINIT);

		requete.type = num_chef;
		requete.client = pid;

		nb_envoi = msgsnd(file_mess_clients, &requete, TAILLE_REQUETE_CLIENT, 0);
		assert(nb_envoi != -1);

		/* Le client attend la réponse du chef */
		nb_lus = msgrcv(file_mess_clients,(void *) &reponse, TAILLE_REPONSE_CHEF, pid, 0);
		assert(nb_lus != -1);

		//afficher la requête et son résultat.
		couleur(BLANC);
		fprintf(stdout, "Le client n°%d a reçu le résultat du travail demandé.\n", pid);
		fprintf(stdout, "\t Résultat obtenu : %d.\n", reponse.resultat);
		couleur(REINIT);
	}

	shmdt(NULL);

	exit(0);

}
