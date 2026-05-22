#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define ITEMS_PER_PRODUCER 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0;

int total_produced = 0;
int total_consumed = 0;
int active_producers;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

/* ---------------- Producer ---------------- */
void *producer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {

        pthread_mutex_lock(&lock);

        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &lock);
        }

        int item = id * 100 + i;

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        total_produced++;

        printf("Producer %d produced %d (count=%d)\n",
               id, item, count);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);

        usleep(50000);
    }

    pthread_mutex_lock(&lock);
    active_producers--;

    if (active_producers == 0) {
        pthread_cond_broadcast(&not_empty);
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

/* ---------------- Consumer ---------------- */
void *consumer(void *arg) {
    int id = *(int *)arg;

    while (1) {

        pthread_mutex_lock(&lock);

        while (count == 0 && active_producers > 0) {
            pthread_cond_wait(&not_empty, &lock);
        }

        if (count == 0 && active_producers == 0) {
            pthread_mutex_unlock(&lock);
            break;
        }

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        total_consumed++;

        printf("Consumer %d consumed %d (count=%d)\n",
               id, item, count);

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);

        usleep(80000);
    }

    return NULL;
}

/* ---------------- Main ---------------- */
int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <producers> <consumers>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    active_producers = N;

    pthread_t producers[N], consumers[M];
    int p_ids[N], c_ids[M];

    for (int i = 0; i < N; i++) {
        p_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &p_ids[i]);
    }

    for (int i = 0; i < M; i++) {
        c_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &c_ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < M; i++) {
        pthread_join(consumers[i], NULL);
    }

    printf("\nTotal produced: %d\n", total_produced);
    printf("Total consumed: %d\n", total_consumed);

    return 0;
}
