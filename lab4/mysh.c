#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

/* ---------------- Parsing ---------------- */
int parse(char *line, char **argv) {
    int argc = 0;
    char *tok = strtok(line, " \t\n");

    while (tok && argc < MAX_ARGS - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\n");
    }

    argv[argc] = NULL;
    return argc;
}

/* ---------------- Main Shell ---------------- */
int main(void) {
    char line[MAX_LINE];
    char *argv[MAX_ARGS];

    /* Shell ignores Ctrl-C */
    signal(SIGINT, SIG_IGN);

    while (1) {

        printf("mysh> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        int argc = parse(line, argv);

        if (argc == 0)
            continue;

        if (strcmp(argv[0], "exit") == 0)
            break;

        /* ---------------- Built-in cd ---------------- */
        if (strcmp(argv[0], "cd") == 0) {
            if (argv[1] == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else {
                chdir(argv[1]);
            }
            continue;
        }

        /* ---------------- Background check ---------------- */
        int background = 0;
        if (strcmp(argv[argc - 1], "&") == 0) {
            background = 1;
            argv[argc - 1] = NULL;
        }

        /* ---------------- Pipe detection ---------------- */
        int pipe_index = -1;
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "|") == 0) {
                pipe_index = i;
                break;
            }
        }

        /* ================= PIPE HANDLING ================= */
        if (pipe_index != -1) {

            argv[pipe_index] = NULL;

            char **cmd1 = argv;
            char **cmd2 = &argv[pipe_index + 1];

            int fd[2];
            pipe(fd);

            pid_t p1 = fork();

            if (p1 == 0) {
                signal(SIGINT, SIG_DFL);

                dup2(fd[1], STDOUT_FILENO);

                close(fd[0]);
                close(fd[1]);

                execvp(cmd1[0], cmd1);
                perror("execvp");
                exit(1);
            }

            pid_t p2 = fork();

            if (p2 == 0) {
                signal(SIGINT, SIG_DFL);

                dup2(fd[0], STDIN_FILENO);

                close(fd[0]);
                close(fd[1]);

                execvp(cmd2[0], cmd2);
                perror("execvp");
                exit(1);
            }

            close(fd[0]);
            close(fd[1]);

            waitpid(p1, NULL, 0);
            waitpid(p2, NULL, 0);

            continue;
        }

        /* ================= REDIRECTION (>) ================= */
        int redirect = -1;

        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], ">") == 0) {
                redirect = i;
                break;
            }
        }

        int out_fd = -1;

        if (redirect != -1) {
            argv[redirect] = NULL;

            out_fd = open(argv[redirect + 1],
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);
        }

        /* ================= NORMAL EXECUTION ================= */
        pid_t pid = fork();

        if (pid == 0) {

            signal(SIGINT, SIG_DFL);

            if (out_fd != -1) {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }

            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        }

        /* Parent */
        if (pid > 0) {

            if (!background) {
                waitpid(pid, NULL, 0);
            } else {
                printf("[running in background pid=%d]\n", pid);
            }

        } else {
            perror("fork");
        }
    }

    return 0;
}
