#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];

    pipe(fd);

    // ---------------- First child: ls ----------------
    if (fork() == 0) {

        dup2(fd[1], STDOUT_FILENO);  // redirect stdout -> pipe write end

        close(fd[0]);
        close(fd[1]);

        execlp("ls", "ls", NULL);
    }

    // ---------------- Second child: wc -l ----------------
    if (fork() == 0) {

        dup2(fd[0], STDIN_FILENO);   // redirect stdin <- pipe read end

        close(fd[0]);
        close(fd[1]);

        execlp("wc", "wc", "-l", NULL);
    }

    // ---------------- Parent ----------------
    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
