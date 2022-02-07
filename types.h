typedef struct {
	long type;
	int duree;
	int nb_1;
    int nb_2;
    int nb_3;
    int nb_4;
	pid_t chef;
	pid_t client;
} requete_chef;

typedef struct {
	long type;
	pid_t client;
} requete_client;

typedef struct {
	long type;
	int resultat;
	pid_t client;
} reponse_mecanicien;

typedef struct {
	long type;
	int resultat;
} reponse_chef;


#define FICHIER_CLE "cle.chefs"
#define TAILLE_REQUETE_CHEF sizeof(requete_chef)-sizeof(long)
#define TAILLE_REQUETE_CLIENT sizeof(requete_client)-sizeof(long)
#define TAILLE_REPONSE_MECANICIEN sizeof(reponse_mecanicien)-sizeof(long)
#define TAILLE_REPONSE_CHEF sizeof(reponse_chef)-sizeof(long)

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