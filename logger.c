#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"

void log(FILE* logf, char *cmd, char* srcurl, char* srcip, char* dsturl, char* dstip, char* msg, char* encrypted) {
    time_t t;
    struct tm* t2;
    t = time(NULL);
    t2 = gmtime(&t);
    fprintf(logf, "%s - %s - %s %s %s %s %s %s\n", asctime(t2), cmd, srcurl, srcip, dsturl, dstip, msg, encrypted);
    fflush(logf);
}

