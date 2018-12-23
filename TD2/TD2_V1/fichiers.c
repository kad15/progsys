/* Affichage a l'ecran du contenu d'un fichier dont le nom est passe en parametre */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
  int fd, nb;
  char buf[512];

  if (argc!=2) {
    write(2, "Nb arguments incorrect\n",30);
    exit(1);
  }

  if ((fd=open(argv[1], O_RDONLY))==-1) {
    perror("Ouverture");
    exit(1);
  }

  while((nb=read(fd, buf, sizeof(buf)))>0) {
	write(1,buf,nb);
	}

  close(fd);

  return(0);
}


/* La commande cat!!! */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
  int fd, nb;
  char buf[512];

  while((nb=read(0, buf, sizeof(buf)))>0) {
	write(1,buf,nb);
	}

  return(0);
}



/* Lancement de ls et stockage de son resultat dans le fichier toto. Revient � faire ls>toto */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
  int fd;

  /* Ouverture fichier toto */

  if ((fd=open("toto", O_WRONLY|O_CREAT|O_TRUNC,0644))==-1) {
    perror("Ouverture toto");
    exit(1);
  }

  /* Fermeture de la sortie standard */
  close(1);

  /* Redirection de toto sur la sortie standard */

  if (dup(fd) == -1) {
    perror("Duplication");
    exit(1);
  }

  /* Fermeture du descripteur fd inutile */

  close(fd);

  /* Recouvrement du process courant par le code de ls */

  execlp("ls", "ls", NULL);

  perror("Lancement ls");

  return(1);
}
