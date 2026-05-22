#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t got_signal = 0;

void handler(int sig) {
    got_signal = sig;
}

int main(void) {

    signal(SIGINT, handler);

    printf("PID %d. Press Ctrl-C.\n", getpid());

    while (!got_signal) {
        sleep(1);
        printf("Still running...\n");
    }

    printf("Got signal %d, exiting\n", got_signal);

    return 0;
}
