#include <stdio.h>
#include <unistd.h>

int main(void) {
	printf("Before fork, PID = %d\n", getpid());
	fork();
	printf("After fork, PID = %d\n", getpid());
	return 0;
}
