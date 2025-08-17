#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 40

int A[SIZE][SIZE], B[SIZE][SIZE];
int sum[SIZE][SIZE], diff[SIZE][SIZE], mult[SIZE][SIZE];

double get_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

void *sum_matrices(void *arg) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            sum[i][j] = A[i][j] + B[i][j];
    pthread_exit(NULL);
}

void *diff_matrices(void *arg) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            diff[i][j] = A[i][j] - B[i][j];
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    // Llenar matrices A y B con 0 o 1
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 2;
            B[i][j] = rand() % 2;
        }

    pthread_t t1, t2;
    double start = get_time();

    // Crear hilos
    pthread_create(&t1, NULL, sum_matrices, NULL);
    pthread_create(&t2, NULL, diff_matrices, NULL);

    // Esperar hilos
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Multiplicar resultados
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            mult[i][j] = 0;
            for (int k = 0; k < SIZE; k++)
                mult[i][j] += sum[i][k] * diff[k][j];
        }

    double end = get_time();
    printf("Tiempo con hilos: %.6f segundos\n", end - start);

    return 0;
}
