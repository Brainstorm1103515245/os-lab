#include <stdio.h>
#include <unistd.h>

int main(void) {
	pid_t pid = fork();
	if (pid == 0) {
		printf("I am a child. My PID is %d, parent is %d\n", getpid(), getppid());
	}
	else if (pid > 0) {
		printf("I am the parent. My PID is %d, child is %d\n", getpid(), pid);
	}
	else {
		perror("fork failed");
	}
	return 0;
}	
