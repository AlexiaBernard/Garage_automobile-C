#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <assert.h>
#include <time.h>
#include "types.h"

int file_mess_ch_mecanicien;
int file_mess_clients; 
int semid_outils;
int semid_clients;
int semid_mutex;
int shmid;
int nb_chefs;
int nb_clients = 0;
key_t cle_client;
key_t cle_mutex;
int sig=1;

void arret(){
	printf(" ARRET DE TOUT LES PROCESSUS\n");
	sig=2;
	assert(msgctl(file_mess_ch_mecanicien, IPC_RMID, NULL) >= 0);
	assert(msgctl(file_mess_clients, IPC_RMID, NULL) >= 0);
	assert(semctl(semid_outils,0,IPC_RMID) >= 0);
	assert(semctl(semid_clients,0,IPC_RMID) >= 0);
	assert(semctl(semid_mutex,0,IPC_RMID) >= 0);
	assert(shmctl(shmid,IPC_RMID,0) >= 0);
}

void creationIPC(int nb_chefs, int nb_1, int nb_2, int nb_3, int nb_4){
	key_t cle_ch_mecanicien;

		/*------Calcul des clés-------*/

	cle_ch_mecanicien = ftok(FICHIER_CLE,'a');
	assert(cle_ch_mecanicien != -1);

	cle_client = ftok(FICHIER_CLE,'b');
	assert(cle_client != -1);

	cle_mutex = ftok(FICHIER_CLE,'m');
	assert(cle_mutex != -1);

		/*-----------La file de message--------------*/

			/*------Création-------*/

	file_mess_ch_mecanicien = msgget(cle_ch_mecanicien, IPC_CREAT|0666);
	assert(file_mess_ch_mecanicien != -1);

	file_mess_clients = msgget(cle_client, IPC_CREAT|0666);
	assert(file_mess_clients != -1);

		/*-----------Sémaphores--------------*/

				/*------Création-------*/

	semid_outils = semget(cle_ch_mecanicien, 4 ,IPC_CREAT|0666);
	assert(semid_outils != -1);

	semid_clients = semget(cle_client, nb_chefs ,IPC_CREAT|0666);
	assert(semid_clients != -1);

	semid_mutex = semget(cle_mutex, 1, IPC_CREAT|0666);
	assert(semid_mutex != -1);

				/*------Initialisation-------*/
	//ici initialisation d'un semaphore pour les outils
	assert(semctl(semid_outils,0,SETVAL,nb_1) >=0); //initialise à "nb_1" qui est le nombre d outil de cette categorie
	assert(semctl(semid_outils,1,SETVAL,nb_2) >=0);
	assert(semctl(semid_outils,2,SETVAL,nb_3) >=0);
	assert(semctl(semid_outils,3,SETVAL,nb_4) >=0);

	//ici initialisation d'un semaphore pour les clients
	unsigned short *tab;
	tab = (unsigned short *) malloc((nb_chefs+1)*sizeof(unsigned short));
	for(int i=0; i<nb_chefs; i++){
		tab[i] = 1;
	}

	assert(semctl(semid_clients,0,SETALL,tab) >=0); //Initialise à 0 pour avoir le nb d'attente

	//initialise mutex
	assert(semctl(semid_mutex,0,SETVAL,1) >=0);

	//Memoire partagée
	shmid = shmget(cle_client,nb_chefs*sizeof(int),IPC_CREAT|0666);
	assert(shmid >= 0);
}


int main(int argc, char const *argv[]){
	pid_t p ;

	signal(SIGINT,arret);

	/*------------Vérification des arguments---------------*/

	if (argc<6){
		fprintf(stderr, "Attention, il faut 6 arguments : nb_chefs nb_mecaniciens nb_1 nb_2 nb_3 nb_4\n");
		exit(-1);
	}

	nb_chefs = (int)strtol(argv[1], NULL, 0);
	int nb_mecaniciens = (int)strtol(argv[2], NULL, 0);
	int nb_1 = (int)strtol(argv[3], NULL, 0);
	int nb_2 = (int)strtol(argv[4], NULL, 0);
	int nb_3 = (int)strtol(argv[5], NULL, 0);
	int nb_4 = (int)strtol(argv[6], NULL, 0);

	if(nb_chefs<2){
		fprintf(stderr, "Attention, il faut au moins deux chefs d'ateliers\n");
		exit(-1);
	}

	if (nb_mecaniciens<3){
		fprintf(stderr, "Attention, il faut au moins trois mécaniciens\n");
		exit(-1);
	}

	creationIPC(nb_chefs, nb_1, nb_2, nb_3, nb_4);


	/*------------Lancer les chefs d'ateliers-------------*/
	char argv2[2];
	char argv3[2];
	char argv4[2];
	char argv5[2];

	snprintf(argv2, sizeof(argv2), "%d", nb_1);
	snprintf(argv3, sizeof(argv3), "%d", nb_2);
	snprintf(argv4, sizeof(argv4), "%d", nb_3);
	snprintf(argv5, sizeof(argv5), "%d", nb_4);


	for(int i=1; i<nb_chefs+1; i++){
		
		char argv1[2];
		snprintf(argv1, sizeof(argv1), "%d", i);
		

		p = fork();
		if(p==0){ //fils
			execl("./chef_atelier", "./chef_atelier", argv1, argv2, argv3, argv4, argv5, NULL);
			break;
		}
	}

	sleep(1);

	/*--------------Lancer les mécaniciens---------------*/
	for(int j=1; j<=nb_mecaniciens; j++){
		char argv1[2];
		snprintf(argv1, sizeof(argv1), "%d", j);
		p = fork();
		if(p==0){ //fils
			execl("./mecanicien", "./mecanicien",argv1,NULL);
			break;
		}
	}

	sleep(1);

	/*--------------Lancer les clients---------------*/
	int duree;
	srand(time(NULL));
	char a1[2];
	snprintf(a1, sizeof(a1), "%d", nb_chefs);
	char a2[200];
	snprintf(a2, sizeof(a2), "%d", cle_client);

	char a3[200];
	snprintf(a3, sizeof(a3), "%d", cle_mutex);

	
	int *file_attente;
	file_attente = (int*) malloc(nb_chefs*sizeof(int));
	for (int i=0; i<nb_chefs; i++){
		file_attente[i] = 0;
	}

	while(sig==1){
		p = fork();
		if(p==0){ //fils
			execl("./client", "./client", a1, a2, a3, NULL);
			break;
		}
		duree = rand()%5;
		sleep(duree);
	}
	
	exit(0);
}
