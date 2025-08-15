#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    int start = 1, end = 10; // Rango de ejemplo
    int suma = 0;

    // Crear el pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Crear el proceso hijo
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- Código del hijo ---
        close(pipefd[0]); // Cierra lectura del pipe

        for (int i = start; i <= end; i++) {
            suma += i;
        }

        // Enviar resultado al padre
        write(pipefd[1], &suma, sizeof(suma));
        close(pipefd[1]); // Cierra escritura
        exit(EXIT_SUCCESS);

    } else {
        // --- Código del padre ---
        close(pipefd[1]); // Cierra escritura del pipe

        // Esperar al hijo
        wait(NULL);

        // Leer resultado
        read(pipefd[0], &suma, sizeof(suma));
        close(pipefd[0]);

        printf("La suma del rango %d a %d es: %d\n", start, end, suma);
    }

    return 0;
}
