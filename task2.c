#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 2
#define ITERATIONS 10000000


int value = 0;

// void *increment(void *arg) {
//     for (int i = 0; i < ITERATIONS; i++) {
//         value++;
//     }
// }

pthread_mutex_t lock;

void *increment(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&lock);
        value++;
        pthread_mutex_unlock(&lock); 
    }
}


// _Atomic int value = 0;

// void *increment(void *arg) {
//     for (int i = 0; i < ITERATIONS; i++) {
//         atomic_fetch_add(&value, 1);
//     }
// }

// pthread_mutex_t lock;
// int iter = 0;
// int value = 0;

// void *increment(void *arg) {
//     int flag = 1;
//     while (flag)
//     {
//         pthread_mutex_lock(&lock);
//         if (iter < ITERATIONS){
//             value++;
//             iter++;
//         }
//         else{
//             flag = 0;
//         }
//         pthread_mutex_unlock(&lock); 
//     }
// }



int main() {
    pthread_t threads[NUM_THREADS];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Очікуване значення змінної: %d\n", ITERATIONS * NUM_THREADS);
    printf("Значення змінної:           %d\n", value);
    printf("Час виконання:              %f сек\n", time_spent);

    return 0;
}

