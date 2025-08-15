#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int n;
    scanf("%d", &n); // Primero leer la cantidad de números
    int *nums = malloc(n * sizeof(int));
    if (!nums) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++)
        scanf("%d", &nums[i]);

    int chunk = n / 3; // Tamaño de bloque por hijo
    int pipefd[3][2];
    pid_t pids[3];

    // Crear 3 pipes y procesos
    for (int i = 0; i < 3; i++) {
        if (pipe(pipefd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            close(pipefd[i][0]); // Cierra lectura

            int start = i * chunk;
            int end = (i == 2) ? n : start + chunk; // último hijo puede tener más
            int suma = 0;
            for (int j = start; j < end; j++)
                suma += nums[j];

            write(pipefd[i][1], &suma, sizeof(suma));
            close(pipefd[i][1]);
            free(nums);
            exit(EXIT_SUCCESS);
        }
        close(pipefd[i][1]); // Padre cierra escritura
    }

    // Padre recibe resultados
    int total = 0, parcial;
    for (int i = 0; i < 3; i++) {
        read(pipefd[i][0], &parcial, sizeof(parcial));
        total += parcial;
        close(pipefd[i][0]);
        waitpid(pids[i], NULL, 0);
    }

    printf("%d\n", total);
    free(nums);
    return 0;
}
