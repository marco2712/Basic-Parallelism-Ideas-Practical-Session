#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//decrypt_it.c
char decrypt_char(char c, int k) {
    if (isalpha((unsigned char)c)) {
        char base = isupper(c) ? 'A' : 'a';
        return (char)(((c - base - k) % 26 + 26) % 26 + base);
    }
    return c;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <rotacion> \"texto\"\n", argv[0]);
        return 1;
    }
    int k = atoi(argv[1]);
    char *text = argv[2];
    for (size_t i = 0; i < strlen(text); i++)
        text[i] = decrypt_char(text[i], k);
    printf("%s\n", text);
    return 0;
}
