#include "md5.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>

#define MD5_LEN 32

pid_t childPID;
using namespace std;

int fd1[2];

void sig_handler(int signo)
{
  char MD5_ENC[MD5_LEN];
  read(fd1[0], MD5_ENC, MD5_LEN+1);
  close(fd1[0]);
  printf("encrypted by process %d : %s\n",childPID,MD5_ENC);
  kill(childPID,SIGINT);
  exit(0);
}


int main() {
	printf("plain text: ");
	char input[20];
	scanf("%s", input);

	pipe(fd1);
	pid_t parentPID = getpid();
	childPID = fork();

	if(childPID == 0) {
		std:string MD5_ENC = md5(input);
		int n = MD5_ENC.length();
		char MD5_char[n+1];
		close(fd1[0]);
		strcpy(MD5_char, MD5_ENC.c_str());  
		write(fd1[1], MD5_char , MD5_LEN+1);
		close(fd1[1]);
		kill(parentPID,SIGHUP);
		while(1){} 
	}
	else {
		signal(SIGHUP,sig_handler);
		while(1){}
	}
}
