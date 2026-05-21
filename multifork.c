#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int i;

    // Create 3 children
    for (i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("fork");
            exit(1);
        }
        else if (pid == 0) {
            // Child process
            printf("Child %d: PID = %d\n", i, getpid());
            exit(0);
        }
    }

    // Parent waits for all children
    for (i = 0; i < 3; i++) {
        wait(NULL);
    }

    printf("All done.\n");

    return 0;
}
