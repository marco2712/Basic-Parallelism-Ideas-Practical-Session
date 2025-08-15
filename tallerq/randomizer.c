#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s n\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    srand(time(NULL));

    printf("%d\n", n); // Primero imprime la cantidad
    for (int i = 0; i < n; i++)
        printf("%d ", rand() % 100);
    printf("\n");

    return 0;
}
