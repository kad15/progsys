/**********************************************************************/
/*		client socket                                         */
/* BELDJILALI IENAC15_SITA  TD9 */
/* gcc -Wall -W -o client9 client9.c */

/* Côté Client (demandeur de la connexion)
: le socket est dit actif
•

crée un socket socket()
se connecte à une <adresse,port> connect()
lit et écrit dans le socket read(),recv();write(),send()
ferme le socket close()
*/

/**********************************************************************/


#include "td9.h"


// 1 - effectue une demande de connexion
// 2 - lit le contenu du fichier fic.large
// 3 - l'envoie au serveur
// 4 - ferme la connexion qd toutes les données ont été lues et envoyées
// 5 - se termine

int main (void)
{
    int fd,nb;
    char buf[BUF_SIZE];
    if ((fd=open("fic.large", O_RDONLY))==-1) {
        perror("Oooops ");
        exit(1);
    }

    char nomMachine[30];

    int sock;
    struct sockaddr_in sin;
    bzero(&sin,sizeof(sin));
    struct hostent *host;

    /* Creation d'une socket aun iveau du client */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
        perror("erreur socket()");
        fflush(stdin);
        getchar();
        exit(1);
    }


    /* On recupere le nom de la machine (pour test en local) */
    gethostname(nomMachine, 30);
    printf("Le nom de la machine locale est :%s: \n", nomMachine);

    /* On recupere l'IP a partir du nom de machine */
    if ((host=gethostbyname(nomMachine)) == NULL) {
        perror("erreur gethostbyname()");
        close(sock);
        fflush(stdin);
        getchar();
        exit(1);
    }

    memcpy(&(sin.sin_addr.s_addr) , host->h_addr_list[0],sizeof(u_long));
    //sin.sin_addr.s_addr = inet_addr("127.0.0.1");  // en loopback pour test!!!
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT_SERVEUR);

    /* Connexion du socket client sock */
    /*
           int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
           L'appel système connect() connecte la socket associée au descripteur de
           fichier sockfd  à  l'adresse  indiquée  par  addr.  L'argument  addrlen
           indique  la  taille  de addr. Le format de l'adresse addr est déterminé
           par la famille de la socket sockfd ; consultez socket(2) pour  plus  de
           détails.
    */
    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
        perror("erreur connect()");
        fflush(stdin);
        getchar();
        exit(1);
    }

    printf("Connexion effectuee \n");

    /* Emission du message sur la socket */


    while((nb=read(fd, buf, sizeof(buf)))>0) {
        write(sock,buf,nb);
    }


    close(fd);
    close(sock);

    printf("Bye!!!\n");
    fflush(stdin);
    getchar();

    return 0;

}


