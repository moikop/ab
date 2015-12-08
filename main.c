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
    FILE* logf;
    FILE* dns_file;
    char cmd[100];
    char file_dns[100] = "dns.txt";
    char line[300];
    char logfile = "log.txt";
    char msg[300];
    char offset;
    char ip_origen[IP_MAX];
    char ip_destino[IP_MAX];
    int loaded;
    tdns dns;
    tdomain td;

    /*validacion parámetros de entrada*/
    if(validateInput(argv,&cmd)!=RES_OK) {
        showHelp();
        return RES_ERROR;
    }

    logf = fopen(logfile,"a");
    if(!logf) return RES_ERROR;

    if(createDNS(&dns,sizeof(tdomain))!=RES_OK) return RES_ERROR;

    /*procesamos el archivo con las direcciones y dominios; en el proceso */
    loaded = loadTree(&dns,file_dns);
    if(loaded!=RES_OK) return loaded;

    /*Decidimos que accion es la que nos pide el usuario*/

    switch(argv[1]) {
        case CMD_SEND:
            if(urlExists(dns.ab,argv[2])!=RES_OK) return RES_ERROR;
            if(urlExists(dns.ab,argv[4])!=RES_OK) return RES_ERROR;
            /* obtengo el elemento del origen y offset = td.offset*/
            getoffset(argv[2],&offset);
            encryptMsg(msg,offset);
            log(logf,CMD_SEND,argv[2],"",argv[3],"",argv[4],msg);
            printf("Mensaje encriptado: %s\n",msg);
            break;
        case CMD_GETIP:
            if(urlExists(dns.ab,argv[2])!=RES_OK) return RES_ERROR;
            if(urlExists(dns.ab,argv[4])!=RES_OK) return RES_ERROR;
            /* obtengo los elementos de ambos y me copia las ips*/
            log(logf,CMD_GETIP,argv[2],ip_origen,argv[3],ip_destino,"","");
            printf("Origen: %s %s\nDestino: %s %s\n",argv[2],ip_origen,argv[3],ip_destino);
            break;
        case CMD_ADDDOMAIN:
            if(urlExists(dns.ab,argv[2])==RES_OK) {
                printf("Ya existe %s.\n",argv[2]);
                return RES_ERROR;
            }
            strcpy(td.domain,argv[2]);
            strcpy(td.ip,argv[3]);
            genoffset(td.domain);
            getoffset(td.domain,&(td.offset));
            AB_Crear(&(td.subab),sizeof(tdomain));
            if(addDomain(&dns,argv[2],&td)!=RES_OK) return RES_ERROR;
            log(logf,CMD_ADDDOMAIN,argv[2],argv[3],"","","","");
            printf("Se agrego %s con direccion ip %s.\n",argv[2],argv[3]);
            break;
        case CMD_DELETEDOMAIN:
            if(urlExists(dns.ab,argv[2])!=RES_OK) {
                printf("No existe %s para eliminar.\n",argv[2]);
                return RES_ERROR;
            }
            /* primitiva para eliminar */
            log(logf,CMD_DELETEDOMAIN,argv[2],ip_origen,"","","","");
            printf("Se elimino a %s con ip %s.\n",argv[2],ip_origen);
            break;
        default:
            printf("Comando equivocado.\n");
            showHelp();
            break;
    }

    fclose(logf);

    return RES_OK;
}
