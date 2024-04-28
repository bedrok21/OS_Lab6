#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 16

#define N 1000
#define M 20000
#define K 200

int A[N][M];
int B[M][K];

int result[N][K];

struct thread_data {
    int start_row;
    int end_row;
};

void *multiply_rows(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int start_row = data->start_row;
    int end_row = data->end_row;

    for (int row = start_row; row <= end_row; row++) {
        for (int col = 0; col < K; col++) {
            int sum = 0;
            for (int i = 0; i < M; i++) {
                sum += A[row][i] * B[i][col];
            }
            result[row][col] = sum;
            //printf("[%d, %d] = %d\n", row, col, sum);
        }
    }

    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = rand() % 10 + 1;
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            B[i][j] = rand() % 10 + 1;
        }
    }

    int num_threads = (MAX_THREADS < N * K) ? MAX_THREADS : N * K;

    pthread_t threads[num_threads];
    struct thread_data thread_data_array[num_threads];

    int thread_count = 0;
    int rows_per_thread = N / num_threads;

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        thread_data_array[i].start_row = i * rows_per_thread;
        thread_data_array[i].end_row = (i == num_threads - 1) ? (N - 1) : (thread_data_array[i].start_row + rows_per_thread - 1);
        pthread_create(&threads[thread_count], NULL, multiply_rows, (void *)&thread_data_array[i]);
        thread_count++;
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Час виконання: %f сек\n", time_spent);

    return 0;
}
