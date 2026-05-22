#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0, out = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    for (int i = 0; i < 20; i++) {

        pthread_mutex_lock(&lock);

        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &lock);
        }

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Produced %d (count=%d)\n", i, count);

        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&lock);

        usleep(50000);
    }

    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 20; i++) {

        pthread_mutex_lock(&lock);

        while (count == 0) {
            pthread_cond_wait(&not_empty, &lock);
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumed %d (count=%d)\n", item, count);

        pthread_cond_signal(&not_full);

        pthread_mutex_unlock(&lock);

        usleep(80000);
    }

    return NULL;
}

int main(void) {
    pthread_t p, c;

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}
