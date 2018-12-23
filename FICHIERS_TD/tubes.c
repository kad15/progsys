
/* ps | wc -l */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  int i=0, tube[2];

  if (pipe(tube) == -1) {
    perror("Creation pipe impossible");
    exit(1);
  }

  if ((i=fork())==-1){
    perror("fork");
    exit(1);
  }

  if (i==0) {
    /* process fils */
    close(1);       /* fermeture sortie std */
    dup(tube[1]);   /* redirection sortie std sur descripteur en ecriture du tube */
    close(tube[0]);  /* fermeture du descripteur inutilise */
    close (tube[1]); /* inutile de garder 2 descripteurs ouverts pour le meme fichier
		      tout acces sur la sortie std va deja  dans le tube */
    execlp("ps", "ps", NULL);
    perror("ps");
  }

  else {
    /* process pere */
    close(0);
    dup(tube[0]);
    close(tube[0]);
    close (tube[1]);
    execlp("wc", "wc","-l", NULL);
    perror("wc");
  }
  return 0;
}




/* programme appele avec "vendeur" ou le texte de la commande  en parametre*/
/* "vendeur" :le process cree un tube appele "CARNET", l'ouvre en lecture, recupere la commande du client et l'affiche */
/* commande : le process ouvre le tube nomme "CARNET" en ecriture et y inscrit la commande */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>  /* pour mkfifo */


int main(int argc, char **argv) {

  int fd;
  char commande[512];
  FILE *in;

  if (argc <2) {
    printf("Nombre d'arguments invalide \n");
    exit(1);
  }

  if (strcmp(argv[1], "vendeur") == 0) {  /* code du vendeur */

    if (mkfifo("CARNET", 0600)==-1) {   /* creation du tube */
      perror("vendeur: creation CARNET");
      exit(1);
    }

    if ((fd=open("CARNET", O_RDONLY))==-1) { /* ouverture du tube en lecture */
      perror("vendeur: ouverture CARNET");
      exit(1);
    }

    if ((in=fdopen(fd, "r"))==NULL) { /* ouverture d'un flux associe a fd */
      perror("vendeur: ouverture flux CARNET");
      exit(1);
    }

    fscanf(in, "%s", commande);
    printf("Recu commande : %s\n", commande);

    fclose(in);
    unlink("CARNET"); /* suppression du fifo */
  }

  else {   /* code du client */

    if ((fd=open("CARNET", O_WRONLY))==-1) {
      perror("client: ouverture CARNET");
      exit(1);
    }

    if (write(fd, argv[1], strlen(argv[1]))!= strlen(argv[1])) {
      perror("client: ecriture CARNET");
      exit(1);
    }
    close(fd);
  }
  return 0;
}


/* Exemple de lancement:

$./a.out vendeur &
$./a.out chocolat
Recu commande : chocolat

*/

