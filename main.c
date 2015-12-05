#include <stdio.h>
#include <stdlib.h>

#include "tda_dns.h"

#define ARGS_DNS_SEND 6
#define ARGS_DNS_GET_IP 5
#define ARGS_DNS_ADD_DOMAIN 5
#define ARGS_DNS_DELETE_DOMAIN 4
#define CMD_SEND    "-dnsSend"
#define CMD_GETIP   "-dnsGetIP"
#define CMD_ADDDOMAIN   "-dnsAddDomain"
#define CMD_DELETEDOMAIN    "-dnsDeleteDomain"

int main(int argc, char** argv) {
    FILE *logf;

    char cmd[100];

    if(validateInput(argv,&cmd)!=RES_OK) {
        showHelp();
        return RES_ERROR;
    }


    free(cmd);
    return RES_OK;
}
