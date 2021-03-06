#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>

void pauseFunc();
void quitFunc();
//Function Definition
struct termios attr;
int pauseVal;
int main(int ac, char * av[]) {
	int counter;
	pauseVal = 0;
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
	
	//turn off echo
	if(tcgetattr(0,&attr) == -1) {
		perror("Cannot get attributes from tty driver\n");
		exit(1);
	}
	attr.c_lflag &= ~ECHO;		//turn off echo bit
	if (tcsetattr(0,TCSANOW, &attr) == -1) {
		perror("Cannot set attributes from tty driver\n");
		exit(1);
	}
	//Set signals
	signal(SIGINT, pauseFunc);
	signal(SIGQUIT,quitFunc);
	
	while (counter >= 0) {
		while(pauseVal) { }
		printf("%d\n", counter);
		counter--;
		sleep(1);
	}
	
	//Restore default settings for terminal
	if(tcgetattr(0,&attr) == -1) {
		perror("Cannot get attributes from tty driver\n");
		exit(1);
	}
	attr.c_lflag |= ECHO;		//turn on echo bit
	if (tcsetattr(0,TCSANOW, &attr) == -1) {
		perror("Cannot set attributes from tty driver\n");
		exit(1);
	}
	return 0;
}

void pauseFunc() {
	if (pauseVal == 0)
		pauseVal = 1;
	else 
		pauseVal = 0;
}
void quitFunc() {
	//Restore default settings for terminal
	if(tcgetattr(0,&attr) == -1) {
		perror("Cannot get attributes from tty driver\n");
		exit(1);
	}
	attr.c_lflag |= ECHO;		//turn on echo bit
	if (tcsetattr(0,TCSANOW, &attr) == -1) {
		perror("Cannot set attributes from tty driver\n");
		exit(1);
	}
	exit(0);
}

