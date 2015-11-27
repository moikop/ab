#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <time.h>

void log(FILE *logf, char *cmd, char* srcurl, char* srcip, char* dsturl, char* dstip, char* msg, char* encrypted) {
    time_t time = time(NULL);
    fprintf(logf, "%s %s %s %s %s %s %s %s\n", asctime(time), cmd, srcurl, srcip, dsturl, dstip, msg, encrypted);
}

#endif // LOGGER_H_INCLUDED
