typedef struct {
	int duree;
	int nb_1;
    int nb_2;
    int nb_3;
    int nb_4;
	pid_t chef;
} requete_chef;

typedef struct {
    pid_t client;
} requete_client;

typedef struct {
	int resultat; //pas sûre du type ici
} reponse_t;


#define FICHIER_CLE_CHEFS "cle.chefs"

/* Couleurs */
#define couleur(param) fprintf(stdout,"\033[%sm",param)
#define NOIR  "30"
#define ROUGE "31"
#define VERT  "32"
#define JAUNE "33"
#define BLEU  "34"
#define CYAN  "36"
#define BLANC "37"
#define REINIT "0"