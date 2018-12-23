/* Affichage par le processus de ses: PID, PPID, GID, EGID, UID, EUID  */

#include <unistd.h>
#include <stdio.h>

int main(){
  
  printf("Mon identifiant (PID) est %d\n", getpid());
  printf("L'identifiant de mon pere(PPID) est %d\n", getppid());
  printf("Mon utilisateur reel (UID) est %d\n", getuid());
  printf("Mon utilisateur effectif (EUID) est %d\n", geteuid());
  printf("Mon groupe reel (GID) est %d\n", getgid());
  printf("Mon groupe effectif (EGID) est %d\n", getegid());

  return(0);
}

/* Resultat obtenu 

   Mon identifiant (PID) est 2306
   L'identifiant de mon pere(PPID) est 2078
   Mon utilisateur reel (UID) est 501
   Mon utilisateur effectif (EUID) est 501
   Mon groupe reel (GID) est 501
   Mon groupe effectif (EGID) est 501
*/

/* Affichage de l'environnement d'un processus */

#include <stdio.h>

int main(int argc, char** argv, char** envp){
  int i=0;
  for (i=0;envp[i]!=NULL; i++)
    printf("%s\n",envp[i]);
  printf("\n%d variables d'environnement initialis�es\n",i);
  return(0);
}


/* Creation de processus fils */

#include <stdio.h>
#include <unistd.h>  /* pour fork() */
#include <stdlib.h>  /* pour exit() */
#include <sys/wait.h> /* pour wait */
#include <signal.h>  /* pour sleep */

int main(void){
  int pid1=0, pid2=0;
  int status;  /* pour code retour wait()*/

  if ((pid1=fork()) == -1){
    perror("Creation fils 1");
    exit(1);
  }

  if (pid1 == 0) {
    /* Code du fils 1 */
    printf("\nJe suis le fils 1 de PID  %d, mon pere est %d\n", getpid(), getppid());
    printf("\nFils 1 vous dit Au revoir!!!\n");
    exit(0);
  }

  /* Code du pere */

  if ((pid2=fork()) == -1){
    perror("Creation fils 2");
    exit(1);
  }

 if (pid2 == 0) {
    /* Code du fils 2 */
    printf("\nJe suis le fils 2 de PID  %d, mon pere est %d\n", getpid(), getppid());
    printf("\nFils 2 vous dit Au revoir!!!\n");
    exit(0);
  }

/* Code du pere */

 printf("Je suis le pere de PID %d et j'ai 2 fils %d et %d \n", getpid(), pid1, pid2);

 wait(&status);
 wait(&status);

  
  return(0);
}


/* Creation de processus fils 
 Adoption du fils par init */

#include <stdio.h>
#include <unistd.h>  /* pour fork() */
#include <stdlib.h>  /* pour exit() */
#include <sys/wait.h> /* pour wait */
#include <signal.h>  /* pour sleep */

int main(void){
  int pid1=0, pid2=0;

  if ((pid1=fork()) == -1){
    perror("Creation fils 1");
    exit(1);
  }

  if (pid1 == 0) {
    /* Code du fils 1 */
    printf("\nJe suis le fils 1 de PID  %d, mon pere est %d\n", getpid(), getppid());
    sleep(2);
    printf("\nFils1 : Mon pere est %d \n", getppid()); 
    printf("\nFils 1 vous dit Au revoir!!!\n");
    exit(0);
  }

  /* Code du pere */

  if ((pid2=fork()) == -1){
    perror("Creation fils 2");
    exit(1);
  }

 if (pid2 == 0) {
    /* Code du fils 2 */
    printf("\nJe suis le fils 2 de PID  %d, mon pere est %d\n", getpid(), getppid());
    sleep(3);
    printf("\nFils2 : Mon pere est %d \n", getppid());
    printf("\nFils 2 vous dit Au revoir!!!\n");
    exit(0);
  }

/* Code du pere */

 printf("Je suis le pere de PID %d et j'ai 2 fils %d et %d \n", getpid(), pid1, pid2);
 sleep(1);
 printf("\nPere vous dit Au revoir!!!\n");
 

  
  return(0);
}


/* Remplacement du code du processus : exec() */ 
 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
 
int main(int argc, char** argv) { 
 
  if (argc != 3){ 
    printf("Nb arguments invalide \n"); 
    exit(1); 
  } 
  
  switch(argv[1][0]) { 
  case 'c': 
    execl("/usr/bin/gzip", "gzip", argv[2], NULL); 
    perror("lancement compress"); 
    exit(1); 
 
  case 'd': 
    execl("/usr/bin/gunzip", "gunzip", argv[2], NULL); 
    perror("lancement uncompress"); 
    exit(1); 
 
  default: 
    printf("argument invalide \n"); 
    exit(1); 
  } 
  return 0;
} 

