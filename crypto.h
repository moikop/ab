#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CRYPTO_DIR "./crypto/"
#define MAX_OFFSET 20

void encryptMsg(char *msg, char offset) {
    int i;

    for (i = 0; i < strlen(msg); i++)
        msg[i] += offset;
}

void decryptMsg(char *msg, char offset) {
    int i;

    for (i = 0; i < strlen(msg); i++)
        msg[i] -= offset;
}

int generateoffset(char *domain) {
    FILE *crypto;
    char *fpath = NULL;
    char offset = 0;

    fpath = malloc(sizeof(char) * (strlen(domain) + strlen(CRYPTO_DIR)));
    if (!fpath) return 0;

    strcpy(fpath, CRYPTO_DIR);
    strcat(fpath, domain);

    crypto = fopen(fpath, "w");
    if (!crypto) return 0;

    srand(time(NULL));
    offset = (rand() % MAX_OFFSET) + 1;

    fprintf(crypto, "%c", offset);
    fclose(crypto);

    free(fpath);
    return 1;
}

char getoffset(char *domain) {
    FILE *crypto;
    char *fpath = NULL;
    char line[5];

    fpath = malloc(sizeof(char) * (strlen(domain) + strlen(CRYPTO_DIR)));
    if (!fpath) return 0;

    strcpy(fpath, CRYPTO_DIR);
    strcat(fpath, domain);

    crypto = fopen(fpath, "r");
    if (!crypto) return 0;

    if (!feof(crypto))
        if (fgets(line, 5, crypto))
            return *line;

    return 0;
}
