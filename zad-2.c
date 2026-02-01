#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 10
#define THREADS 3

int numbers[SIZE];


pthread_mutex_t print_mutex;

void* worker(void* arg)
{
    int id = *(int*)arg;

    /* 3. Każdy wątek przetwarza co trzeci element */
    for (int i = id; i < SIZE; i += THREADS) {
        pthread_mutex_lock(&print_mutex);
        printf("[wątek %d] element [%d] = %d\n", id, i, numbers[i]);
        pthread_mutex_unlock(&print_mutex);
    }

    return NULL;
}

int main(void)
{
    pthread_t threads[THREADS];
    int ids[THREADS];

    srand((unsigned int)time(NULL));

    for (int i = 0; i < SIZE; i++) {
        numbers[i] = rand() % 100;
    }

    /* Inicjalizacja mutexa */
    pthread_mutex_init(&print_mutex, NULL);

    /* 2. Tworzenie wątków */
    for (int i = 0; i < THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    /* 5. Oczekiwanie na zakończenie wątków */
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* 6. Zwolnienie zasobów */
    pthread_mutex_destroy(&print_mutex);

    return 0;
}
