#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 10          // tamaño del buffer
#define NUM_PROD 2    // número de productores
#define NUM_CONS 2    // número de consumidores

int buffer[N];       // buffer compartido
pthread_mutex_t mtx;
pthread_cond_t cond_prod, cond_cons;
int stop = 0; // bandera para detener la ejecución

// Función para obtener un número aleatorio entre min y max
int random_between(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Función del productor
void* productor(void* arg) {
    while (!stop) {
        pthread_mutex_lock(&mtx);

        // buscar la primera posición con 0
        int pos = -1;
        for (int i = 0; i < N; i++) {
            if (buffer[i] == 0) {
                pos = i;
                break;
            }
        }

        if (pos == -1) {
            // buffer lleno -> detener ejecución
            stop = 1;
            pthread_cond_broadcast(&cond_cons);
            pthread_mutex_unlock(&mtx);
            break;
        }

        int val = random_between(1, 9);
        buffer[pos] = val;
        printf("Productor %ld produjo %d en pos %d\n", (long)arg, val, pos);

        pthread_cond_signal(&cond_cons);
        pthread_mutex_unlock(&mtx);

        sleep(random_between(1, 3)); // simula tiempo de producción
    }
    pthread_exit(NULL);
}

// Función del consumidor
void* consumidor(void* arg) {
    while (!stop) {
        pthread_mutex_lock(&mtx);

        // buscar la primera posición con valor distinto de 0
        int pos = -1, val = 0;
        for (int i = 0; i < N; i++) {
            if (buffer[i] != 0) {
                pos = i;
                val = buffer[i];
                break;
            }
        }

        if (pos == -1) {
            if (stop) {
                pthread_mutex_unlock(&mtx);
                break;
            }
            pthread_cond_wait(&cond_cons, &mtx);
            pthread_mutex_unlock(&mtx);
            continue;
        }

        buffer[pos] = 0;
        printf("Consumidor %ld consumió %d en pos %d\n", (long)arg, val, pos);

        pthread_cond_signal(&cond_prod);
        pthread_mutex_unlock(&mtx);

        sleep(val); // simula tiempo de consumo
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    pthread_t prod[NUM_PROD], cons[NUM_CONS];

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond_prod, NULL);
    pthread_cond_init(&cond_cons, NULL);

    // inicializar buffer en ceros
    for (int i = 0; i < N; i++) buffer[i] = 0;

    // crear productores
    for (long i = 0; i < NUM_PROD; i++)
        pthread_create(&prod[i], NULL, productor, (void*)i);

    // crear consumidores
    for (long i = 0; i < NUM_CONS; i++)
        pthread_create(&cons[i], NULL, consumidor, (void*)i);

    // esperar hilos
    for (int i = 0; i < NUM_PROD; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < NUM_CONS; i++) pthread_join(cons[i], NULL);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond_prod);
    pthread_cond_destroy(&cond_cons);

    printf("Ejecución terminada.\n");
    return 0;
}
