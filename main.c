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

int processData(tdns* dns,char** argv,char* cmd,FILE** logf) {

    tdomain data;
    tdomain td;
    char ip_origen[IP_MAX];
    char ip_destino[IP_MAX];
    char msg[300];

    if(strcmp(argv[1],CMD_SEND)==0) {
        if(urlExists(*dns,argv[2])!=RES_OK) return RES_ERROR;
        if(urlExists(*dns,argv[4])!=RES_OK) return RES_ERROR;
        getValue(dns,argv[2],&data);
        strcpy(ip_origen,data.ip);
        getValue(dns,argv[4],&data);
        strcpy(ip_destino,data.ip);
        encryptMsg(msg,data.offset);
        log(logf,CMD_SEND,argv[2],ip_origen,argv[3],ip_destino,argv[4],msg);
        printf("Mensaje encriptado: %s\n",msg);
    }
    else if(strcmp(argv[1],CMD_GETIP)==0) {
        if(urlExists(*dns,argv[2])!=RES_OK) return RES_ERROR;
        if(urlExists(*dns,argv[4])!=RES_OK) return RES_ERROR;
        getValue(dns,argv[2],&data);
        strcpy(ip_origen,data.ip);
        getValue(dns,argv[4],&data);
        strcpy(ip_destino,data.ip);
        log(logf,CMD_GETIP,argv[2],ip_origen,argv[3],ip_destino,"","");
        printf("Origen: %s %s\nDestino: %s %s\n",argv[2],ip_origen,argv[3],ip_destino);
    }
    else if(strcmp(argv[1],CMD_ADDDOMAIN)==0) {
        if(urlExists(*dns,argv[2])==RES_OK) {
            printf("Ya existe %s.\n",argv[2]);
            return RES_ERROR;
        }
        strcpy(td.domain,argv[2]);
        strcpy(td.ip,argv[3]);
        genoffset(td.domain);
        getoffset(td.domain,&(td.offset));
        AB_Crear(&(td.subab),sizeof(tdomain));
        if(addDomain(dns,argv[2],&td)!=RES_OK) return RES_ERROR;
        log(logf,CMD_ADDDOMAIN,argv[2],argv[3],"","","","");
        printf("Se agrego %s con direccion ip %s.\n",argv[2],argv[3]);
    }
    else if(strcmp(argv[1],CMD_DELETEDOMAIN)) {
        if(urlExists(*dns,argv[2])!=RES_OK) {
            printf("No existe %s para eliminar.\n",argv[2]);
            return RES_ERROR;
        }
        deleteUrl(dns,argv[2]);
        log(logf,CMD_DELETEDOMAIN,argv[2],ip_origen,"","","","");
        printf("Se elimino a %s con ip %s.\n",argv[2],ip_origen);
    } else {
        printf("Comando equivocado.\n");
        showHelp();
        return RES_ERROR;
    }
    return RES_OK;
}


int main(int argc, char** argv) {
    FILE* logf;
    tdns dns;
    char cmd[100];
    char file_dns[100] = "dns.txt";
    char logfile = "log.txt";
    int loaded;
    int error;
    int crear;

    /*validacion parámetros de entrada*/
    if(validateInput(argv,&cmd)!=RES_OK) {
        showHelp();
        return RES_ERROR;
    }

    logf = fopen(logfile,"a");
    if(!logf) {
        printf("No se pudo abrir el archivo de log.\n");
        return RES_ERROR;
    }

    crear = createDNS(&dns,sizeof(tdomain));
    if(crear!=RES_OK) {
        printf("Ocurrió un error: %i.\n",crear);
        return RES_ERROR;
    }

    /*procesamos el archivo con las direcciones y dominios; en el proceso */
    loaded = loadTree(&dns,file_dns);
    if(loaded!=RES_OK) {
        printf("Ocurrió un error: %i.\n",loaded);
        return loaded;
    }

    /*Decidimos que accion es la que nos pide el usuario*/
    error = processData(&dns,argv,cmd,&logf);
    if(error!=RES_OK) {
        printf("Ocurrió un error: %i.\n",error);
        fclose(logf);
        return RES_ERROR;
    }

    fclose(logf);

    return RES_OK;
}
