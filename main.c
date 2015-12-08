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

    switch(argv[1]) {
        case CMD_SEND: break;
        case CMD_GETIP: break;
        case CMD_ADDDOMAIN:
            /*setear una estructura con los datos que se pasan*/
            if(addDomain(&dns,argv[2],const tdomain* td)!=RES_OK) return RES_ERROR;
            break;
        case CMD_DELETEDOMAIN: break;
        default:
    }

    return RES_OK;
}
