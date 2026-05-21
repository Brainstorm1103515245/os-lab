#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	pid_t pid = fork();
	if (pid == 0) {
		execlp("ls", "ls", "-l", NULL);
		perror("exec failed");
		return -1;
	}
	int status;
	waitpid(pid, &status, 0);
	printf("Child exited with status %d\n", WEXITSTATUS(status));
	return 0;
}
