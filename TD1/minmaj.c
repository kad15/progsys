#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>



int main(){

	int nb,i;
	char buf[512];
	int d='a'-'A';
	

 	while((nb=read(0, buf, sizeof(buf)))>0) {
		for (i=0; i < nb; i++){
			if (buf[i]>='a'&& buf[i]<='z')
				buf[i]-=d;
		}
		write(1,buf,nb);
	}



	return EXIT_SUCCESS;
}
