#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<sys/types.h>

int main(int ac, char * av[]) {
	int counter;
	struct termios attr;
	
	if (ac != 2) {
		printf("SYNOPSIS: timer NUMBER\n");
		return 0;
	}
	int i;
	for (i = 0; i < strlen(av[1]);i++) {
		if (!isdigit(av[1][i])) {
			printf("SYNOPSIS: timer NUMBER\n");
			return 0;
		}
	}
	counter = atoi(av[1]);
	
	//Set control keys and turn off echo
	if(tcgetattr(0,&attr) == -1) {
		perror("Cannot get attributes from tty driver\n");
		exit(1);
	}
	int temp1 = attr.c_cc[VSTART]; //back up current control keys
	int temp2 = attr.c_cc[VSTOP];
	attr.c_cc[VSTART] = 'u';	//assign start keystroke
	attr.c_cc[VSTOP] = 'p';		//assign stop keystroke
	attr.c_lflag &= ~ECHO;		//turn off echo bit
	if (tcsetattr(0,TCSANOW, &attr) == -1) {
		perror("Cannot set attributes from tty driver\n");
		exit(1);
	}
	
	while (counter >= 0) {
		printf("%d\n", counter);
		counter--;
		sleep(1);
	}
	
	//Restore default settings for terminal
	if(tcgetattr(0,&attr) == -1) {
		perror("Cannot get attributes from tty driver\n");
		exit(1);
	}
	attr.c_cc[VSTART] = temp1;	//restore original control key
	attr.c_cc[VSTOP] = temp2;
	attr.c_lflag |= ECHO;		//turn on echo bit
	if (tcsetattr(0,TCSANOW, &attr) == -1) {
		perror("Cannot set attributes from tty driver\n");
		exit(1);
	}

	return 0;
}
