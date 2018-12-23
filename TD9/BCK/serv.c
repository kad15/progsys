/* Affichage a l'ecran du contenu d'un fichier dont le nom est passe en parametre */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int lectureLigne(int fd,char *ligne, int buf_size);
void toMaj(char *buf, int nb);
void service(int nsock);

int service(int nsock)
{
  int nb, i;
  char buf[512];
  int ecart = 'a' - 'A';
  
// Si un nom de fichier est fourni en paramètre, on ouvre ce fichier
// et on lui associe le descripteur sortie standard
	/* copie du descripteur nsock en position 0 de la fd table  */
	/* => la fonction admet en entrée le flux issu du réseau */
	if (dup2(nsock,0)==-1) 
	{
    		perror("dup2");
    		exit(1);
  	}
	close(nsock);

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
} // fin service


int lectureLigne(int fd,char *ligne, int buf_size)
{
	int nb=1;
	char c;
	while(c !='\n' && n<BUF_SIZE)
		if (read(fd,&c,1)==-1)
		{
			perror("erreur lecture ligne");
			exit(1);
		}
		nb++;
		*ligne++;
	}//fin while c !=
	*ligne='\n';
	return(nb);
}//fin lireligne

/***************************************/

void toMaj(char *buf, int nb)
{
	int ecart = 'a' - 'A';
	int i;
	for (i=0;i<nb;i++)
	{
		if (buf[i] >='a' && buf[i]<='z')
				buf[i]-=ecart;
	} // fin for i=0
	
}//fin toMaj





