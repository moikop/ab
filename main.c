#include <stdio.h>
#include <stdlib.h>

#define CMD_SEND    "-dnsSend"
#define CMD_GETIP   "-dnsGetIP"
#define CMD_ADDDOMAIN   "-dnsAddDomain"
#define CMD_DELETEDOMAIN    "-dnsDeleteDomain"

int main(int argc, char *argv[]) {
    FILE *logf;

    if (strcmp(argv[1], CMD_SEND) == 0) {
        /*
        argv[2] == srcurl
        argv[3] == dsturl
        argv[4] == msg
        argv[5] == logger file
        */
    } else if (strcmp(argv[1], CMD_GETIP) == 0) {
        /*
        argv[2] == srcurl
        argv[3] == dsturl
        argv[4] == logger file
        */
    } else if (strcmp(argv[1], CMD_ADDDOMAIN) == 0) {
        /*
        argv[2] == url
        argv[3] == IP
        argv[4] == logger file
        */
    } else if (strcmp(argv[1], CMD_DELETEDOMAIN) == 0) {
        /*
        argv[2] == url
        argv[3] == logger file
        */
    } else {
        /*
        return ERROR_UNKNOWN_CMD
        */
    }

    return 0;
}
