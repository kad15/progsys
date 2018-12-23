#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

// Configuration d un port serie (adaptateur USB/serie du raspberry pi)

int main(){

	struct termios t;
	int fd, nb;
	char c;

// ouverture du port
	if ((fd=open("/dev/ttyUSB0", O_RDONLY|O_NOCTTY))==-1) {
		perror("ouverture liaison serie");
		return -1;
	}

// recuperation de la configuration courante
	if (tcgetattr(fd, &t)==-1) {
		perror("lecture config liaison serie");
		return -1;
	}

// parametres de transmission : 2400 bds, 8 bits data, 2 bits stop, pas de parite
	t.c_cflag = (B2400|CS8|CREAD|CSTOPB|CLOCAL) & ~PARENB & ~CRTSCTS;
	t.c_lflag = t.c_lflag & ~ICANON;

// application de la nouvelle configuration
	if (tcsetattr(fd, TCSANOW, &t)==-1) {
		perror("ecriture config liaison serie");
		return -1;
	}

// ...


	close(fd);


return 0;
}
