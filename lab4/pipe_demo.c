#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];

    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) {
        // Child process

        close(fd[1]);  // close write end

        char buf[100];

        int n = read(fd[0], buf, sizeof(buf) - 1);
        buf[n] = '\0';

        printf("Child received: %s\n", buf);

        close(fd[0]);
    }
    else {
        // Parent process

        close(fd[0]);  // close read end

        const char *msg = "Hello via pipe";

        write(fd[1], msg, strlen(msg));

        close(fd[1]);

        wait(NULL);
    }

    return 0;
}
