#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crypto.h"

#define CRYPTO_DIR "./crypto/"
#define MAX_OFFSET 20
#define MAX_LINE 256

#define RES_OK 0
#define RES_ERROR 1
#define RES_MEM_ERROR -1

int encryptMsg(char *msg, char offset) {
    int i, len = 0;

    len = strlen(msg);

    for (i = 0; i < len; i++)
        msg[i] += offset;
    return RES_OK;
}

int decryptMsg(char *msg, char offset) {
    int i, len = 0;

    len = strlen(msg);

    for (i = 0; i < len; i++)
        msg[i] -= offset;
    return RES_OK;
}

char genoffset(char *domain) {
    FILE *crypto;
    char *fpath = NULL;
    char offset = 0;

    fpath = malloc(sizeof(char) * (strlen(domain) + strlen(CRYPTO_DIR))+1);
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
    return offset;
}

int getoffset(char *domain,char* offset) {
    FILE *crypto;
    char *fpath = NULL;
    char line[5];

    fpath = malloc(sizeof(char)*(strlen(domain)+strlen(CRYPTO_DIR))+1);
    if (!fpath) return RES_MEM_ERROR;

    strcpy(fpath, CRYPTO_DIR);
    strcat(fpath, domain);

    crypto = fopen(fpath, "r");
    if (!crypto) return RES_ERROR;

    if (!feof(crypto)){
        if (fgets(line, 5, crypto)) {
            strcpy(offset,line);
            return RES_OK;
        }
    }
    offset = '\0';
    return RES_ERROR;
}

