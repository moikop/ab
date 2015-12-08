#include <stdio.h>
#include <stdlib.h>

#include "tda_dns.h"
#include "tda_dns.c"

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
    FILE* dns_file;
    char cmd[100];
    char file_dns[100] = "dns.txt";
    char line[300];
    tdns dns;
    int loaded;

    /*validacion parámetros de entrada*/
    if(validateInput(argv,&cmd)!=RES_OK) {
        showHelp();
        return RES_ERROR;
    }

    createDNS(&dns,sizeof(tdomain));

    /*procesamos el archivo con las direcciones y dominios; en el proceso */
    loaded = loadTree(&dns,file_dns);
    if(loaded!=RES_OK) return loaded;

    /*setear una estructura con los datos que se pasan*/

    /*Decidimos que accion es la que nos pide el usuario*/

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
        addDomain(NULL, argv[2], argv[3]);
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

    return RES_OK;
}
