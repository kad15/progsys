/**********************************************************************
		Exemple code client socket

**********************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main (void) {

    char *message = "Bonjour";
	char nomMachine[30];

	int sock;
	struct sockaddr_in sin;
	struct hostent *host;

	/* Creation d'une socket */

	if ((sock = socket(AF_INET, SOCK_STREAM, 0))==-1){
			perror("erreur socket()");
			fflush(stdin);
			getchar();
			exit(0);
	}

	/* On recupere le nom de la machine (pour test en local) */

	gethostname(nomMachine, 30);
	printf("Le nom de la machine locale est :%s: \n", nomMachine);

	/* On recupere l'IP a partir du nom de machine */

	if ((host=gethostbyname(nomMachine)) == NULL){
			perror("erreur gethostbyname()");
			close(sock);
			fflush(stdin);
			getchar();
			exit(0);
	}

	memcpy(&(sin.sin_addr.s_addr) , host->h_addr_list[0],sizeof(u_long));
	//sin.sin_addr.s_addr = inet_addr("127.0.0.1");  // en loopback pour test!!!
	sin.sin_family = AF_INET;
	sin.sin_port = htons(9999); /* port 9999 */

	/* Connexion */

	if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1){
			perror("erreur connect()");
			fflush(stdin);
			getchar();
			exit(0);
	}

	printf("Connexion effectuee \n");

	/* Emission du message sur la socket */

	if (send(sock, message, strlen(message)+1, 0)==-1){
		perror("erreur send()");
		exit(0);
	}

	close(sock);

	printf("Bye!!!\n");
	fflush(stdin);
	getchar();

	return 0;

	}


	#include <stdio.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

/**********************************************************************
		Exemple code serveur socket

**********************************************************************/

#define BUF_SIZE 20

int main (void) {

	int sock, csock;
	struct sockaddr_in sin, csin;
	int nbRecus;
	socklen_t taille;
	char buf [BUF_SIZE];


	// Creation de la socket d ecoute

	if ((sock = socket(AF_INET, SOCK_STREAM, 0))==-1) {
		perror("erreur socket()");
		getchar();
		exit(0);
	}

	// attachement de la socket (toute connexions entrantes acceptees, ecoute sur le port 9999)

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(9999); /* port 9999 */

	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
		perror("erreur bind()");
		close(sock);
		getchar();
		exit(0);
	}

	// une seule connexion a la fois

	if (listen(sock, 1)!=0) {
		perror("erreur listen()");
		close(sock);
		getchar();
		exit(0);
	}

	taille = sizeof(csin);

    // attente d une connexion client

    if ((csock=accept(sock, (struct sockaddr *)&csin, &taille)) == -1){
        perror("erreur accept()");
		close(sock);
		getchar();
		exit(0);
    }

   // lecture des informations recues

    if ((nbRecus=recv(csock, buf, BUF_SIZE, 0))<=0){
        perror("erreur recv()");
		close(sock);
		getchar();
		exit(0);
    }

		// traitement des informations recues

    buf[nbRecus]=0;
    printf("recu :%s:\n", buf);

	return 0;

}
/*-------------------------------------------------------------------------------------*/
