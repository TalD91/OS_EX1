#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#define PROCESS_NUM 5
#define DEBUGGING 1
void sigCatcher(int signo){

        printf("PID %d caught one\n",getpid());
	sleep(1);
        printf("PID %d is dead\n",getpid());
	kill(getpid(),SIGINT);
}

int main() {
	pid_t ppid,wpid;
	int processes[PROCESS_NUM];
	int status = 0;
	for(int i=0;i<PROCESS_NUM;i++) {
		ppid = fork();
		if(ppid == 0) {
			signal(SIGHUP,sigCatcher);
			printf("PID %d ready\n",getpid());
			sleep(2);
			if(i==4) {
				sleep(4);
				printf("killing %d\n",processes[0]);
				kill(processes[0],SIGHUP);
			}
			kill(processes[i+1],SIGHUP);
			while(1){}
		}
	}
	for(int i=0;i<PROCESS_NUM;i++) {
		processes[i]=getpid()+i+1;
	}
	sleep(20);
}
