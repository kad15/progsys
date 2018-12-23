/* Affichage a l'ecran du contenu d'un fichier dont le nom est passe en parametre */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
  int fd, nb, i;
  char buf[512];
  int ecart = 'a' - 'A';
  
// Si un nom de fichier est fourni en paramètre, on ouvre ce fichier
// et on lui associe le descripteur sortie standard

	if (argc==2)
	{
		if ((fd=open(argv[1], O_RDONLY))==-1) 
		{
    			perror("Ouverture");
    			exit(1);
  		}
		close(0);
		dup(fd);
		close(fd); // le descripteur num 1 pointe sur le fichier donné en argument
	
	}

  	while((nb=read(0, buf, sizeof(buf)))>0) 
  	{
		for (i=0;i<nb;i++)
		{
			if (buf[i] >='a' && buf[i]<='z')
			{
				buf[i]-=ecart;
			}
		}
		write(1,buf,nb);
  	}

  return(0);
}
