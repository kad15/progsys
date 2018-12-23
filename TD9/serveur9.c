/**********************************************************************/
/*		serveur socket                                        */

/* BELDJILALI IENAC15_SITA  TD9 */
/* gcc -Wall -W -o serveur9 serveur9.c */

/*
Côté Serveur (en attente de connexion)
: le socket est dit passif

crée un socket socket()
associe une adresse au socket bind()
se met à l ’écoute des connexions entrantes listen()
accepte une connexion entrante accept()
lit et écrit sur le socket read(),recv();write(),send()
ferme le socket close()
*/
/**********************************************************************/

#include "td9.h"



int service(int nsock);



int main (void)
{

    int sock, newsock;
    struct sockaddr_in sin, csin;
    socklen_t taille;



    // Creation de la socket d ecoute en TCP  0 : le systeme choisit le protocole
    // sinon mettre IPPROTO_TCP. Une fois créé le socket n'est connu que du processus
    // qui l'a créé et de ses descendants
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
        perror("erreur socket()");
        getchar();
        exit(1);
    }

    // attachement de la socket (toute connexions entrantes acceptees, ecoute sur le port PORT_SERVEUR)
    // le champ sin_addr contient l'adresse IP associée à la socket
    bzero(&sin,sizeof(sin));
    sin.sin_addr.s_addr = INADDR_ANY; // repr une des adresses de la machine locale
    sin.sin_family = AF_INET; // adresse IPV4
    sin.sin_port = htons(PORT_SERVEUR); /* port sur 16 bits donc conversion d'un short du format host au format réseau host to network short*/
    // bind associe l'adresse locale &sin à l'id du socket sock (c un file descripteur)
    // le socket aura une adresse et pourra être contacté de l'extérieur
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
        perror("erreur bind()");
        close(sock);
        getchar();
        exit(0);
    }

    //  int listen(int sockfd, int backlog);
    //  Le paramètre backlog ici == 10 définit une longueur maximale  pour  la  file  des
    //connexions  en  attente  pour  sockfd
    if (listen(sock, 10)!=0) {
        perror("erreur listen()");
        close(sock);
        getchar();
        exit(0);
    }



    // attente d une connexion client
// #include <sys/types.h>          /* Consultez NOTES */
//      #include <sys/socket.h>

    //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    /*
    	L'appel système accept() est employé avec les sockets utilisant un pro‐
           tocole en mode connecté (SOCK_STREAM, SOCK_SEQPACKET).  Il  extrait  la
           première  connexion  de  la file des connexions en attente de la socket
           sockfd à l'écoute, crée une nouvelle socket et alloue pour cette socket
           un  nouveau  descripteur  de  fichier qu'il renvoie. La nouvelle socket
           n'est pas en état d'écoute. La socket originale sockfd n'est pas  modi‐
           fiée par l'appel système.

    */
    taille = sizeof(csin);
    while(1) {
        printf("serveur9 en attente sur port 9000 !\n");
        if ((newsock=accept(sock, (struct sockaddr *)&csin, &taille)) == -1) {
            perror("erreur accept()");
            close(sock);
            getchar();
            exit(1);
        }//fin if newsock
        service(newsock);
    }//fin while



    return(0);

} // fin main
/*-------------------------------------------------------------------------------------*/
// lecture en boucle des données envoyées par le client
// conversion en majuscule
// affichage écran

int service(int nsock)
{
    int nb, i;
    char buf[BUF_SIZE];
    int ecart = 'a' - 'A';


// Si un nom de fichier est fourni en paramètre, on ouvre ce fichier
// et on lui associe le descripteur sortie standard
    /* copie du descripteur nsock en position 0 de la fd table  */
    /* => la fonction admet en entrée le flux issu du réseau */
    if (dup2(nsock,0)==-1) {
        perror("dup2");
        exit(1);
    }

    close(nsock); // nsock devenu inutile

    //nb = lectureLigne(nsock,buf,BUF_SIZE);
    //toMaj(buf,nb);
    char * mes = "Je suis le serveur9 et voici ce que j'ai reçu/converti en majuscule. \n\n";
    write(1,mes,sizeof(mes));

    while((nb=read(0, buf, sizeof(buf)))>0) {
        for (i=0; i<nb; i++) {
            if (buf[i] >='a' && buf[i]<='z')
                buf[i]-=ecart;

        } // fin for i=0
        write(1,buf,nb);

    }// fin while nb

    return(0);
}//fin service
























