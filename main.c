#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tda_dns.h"
#include "logger.h"
#include "crypto.h"
#include "ab.h"


#define ARGS_DNS_SEND 6
#define ARGS_DNS_GET_IP 5
#define ARGS_DNS_ADD_DOMAIN 5
#define ARGS_DNS_DELETE_DOMAIN 4
#define CMD_SEND    "-dnsSend"
#define CMD_GETIP   "-dnsGetIP"
#define CMD_ADDDOMAIN   "-dnsAddDomain"
#define CMD_DELETEDOMAIN    "-dnsDeleteDomain"

/******************************************* Funciones de validacion *******************************************************/

void showHelp(char* name)
{
    printf("Este programa simula un arbol de dns limitado, y permite simular el envío de mensajes encriptados entre dominios pertenecientes al arbol.\n");
    printf("Uso:\n");
    printf("Muestra en pantalla el mensaje encriptado:\n %s -dnsSend [urlOrigen] [urlDestino] [mensaje] [archivo.log]",name);
    printf("Muestra en pantalla los datos de la IP Origen y Destino:\n %s -dnsGetIP [urlOrigen] [urlDestino] [archivo.log]",name);
    printf("Muestra en pantalla el dominio e IP agregados:\n %s -dnsAddDomain [url] [IP] [archivo.log]",name);
    printf("Muestra en pantalla el dominio e IP eliminados:\n %s -dnsDeleteDomain [url] [archivo.log]",name);
}

int validateOctect(long ip) {
    if ((ip >= 0) && (ip <= 255))
        return RES_OK;
    return RES_ERROR;
}

int validateIP(char* ip) {

    char* token;
    char* ptr;
    long aux;
    int i = 0;

    token = strtok(ip,DOT);
    if(!token) return RES_ERROR;

    /*validación de los octetos de la dirección ip*/
    while(token!=NULL) {
        i++;
        aux = strtol(token,&ptr,10);
        if(validateOctect(aux)!=RES_OK)
            return RES_ERROR;
        token = strtok(NULL,DOT);
    }
    if(i!=4) return RES_ERROR;

    return RES_OK;
}

int validateURL(char* url) {

    int i = 0;
    int length = 0;
    int tag_length = 0;
    char* token;
    char s[2];
    char e[2];
    char ti[2];

    token = strtok(url,DOT);
    if(!token) return RES_ERROR;

    s[1] = '\0';e[1] = '\0';ti[1] = '\0';

    /*validación del dominio*/
    while(token!=NULL) {
        tag_length = strlen(token);
        length += tag_length;
        s[0] = token[0];
        e[0] = token[tag_length-1];

        if(tag_length>DOMAIN_TAG_MAX) return RES_ERROR;
        if((strcmp(s,DASH)==0)||(strcmp(e,DASH)==0)) return RES_ERROR;

        for(i=0;i<tag_length;i++) {
            ti[0] = token[i];
            if((isalpha((int)ti)!=0)||(isdigit((int)ti)!=0)||(strcmp(ti,DASH)==0))
                return RES_ERROR;
        }
        token = strtok(NULL,DOT);
    }

    if(length>DOMAIN_NAME_MAX)
        return RES_ERROR;

    return RES_OK;
}

int validateInput(int argc, char** argv, char** cmd) {

    if(argc!=ARGS_DNS_SEND && argc!=ARGS_DNS_GET_IP && argc!=ARGS_DNS_ADD_DOMAIN && argc!=ARGS_DNS_DELETE_DOMAIN) return RES_ERROR;

    if (strcmp(argv[1],CMD_SEND)==0 && argc==ARGS_DNS_SEND && validateURL(argv[2])==RES_OK && validateURL(argv[3])==RES_OK) {
        strcpy(*cmd,CMD_SEND);
    } else if (strcmp(argv[1],CMD_GETIP)==0 && argc==ARGS_DNS_GET_IP && validateURL(argv[2])==RES_OK && validateURL(argv[3])==RES_OK) {
        strcpy(*cmd,CMD_GETIP);
    } else if (strcmp(argv[1],CMD_ADDDOMAIN)==0 && argc==ARGS_DNS_ADD_DOMAIN && validateURL(argv[2])==RES_OK && validateIP(argv[3])==RES_OK) {
        strcpy(*cmd,CMD_ADDDOMAIN);
    } else if (strcmp(argv[1],CMD_DELETEDOMAIN)==0 && argc==ARGS_DNS_DELETE_DOMAIN && validateURL(argv[2])==RES_OK) {
        strcpy(*cmd,CMD_DELETEDOMAIN);
    } else {
        return RES_ERROR;
    }
    return RES_OK;
}

/**************************************************************************************************************************/

/*********************************** Funciones de procesamiento ***********************************************************/

int loadTree(tdns *dns, char *configFile) {
    FILE *cfile;
    tdomain temp;
    char* buffer;
    char line[MAX_LINE];
    char Ipa[IP_MAX];
    char url[DOMAIN_TAG_MAX];
    int error;

    cfile = fopen(configFile, "r");
    if(!cfile) return RES_ERROR;

    while(!feof(cfile)) {
        if (fgets(line, MAX_LINE-1, cfile)) {
            buffer = strtok(line," ");
            strncpy(url, buffer, DOMAIN_TAG_MAX);
            buffer = strtok(NULL,"");
            strncpy(Ipa, buffer, IP_MAX);
            if(validateURL(url)!=RES_OK && validateIP(Ipa)!=RES_OK) return RES_ERROR;
            strcpy(temp.domain,url);
            strcpy(temp.ip,Ipa);
            temp.offset = genoffset(url);
            AB_Crear(&(temp.subab),sizeof(tdomain));
            error = addDomain(dns,url,&temp);
            if(error!=RES_OK) {
                fclose(cfile);
                return error;
            }
        }
    }
    fclose(cfile);
    return RES_OK;
}

int processData(tdns* dns,char** argv,char* cmd,FILE* logf) {

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
        showHelp(argv[0]);
        return RES_ERROR;
    }
    return RES_OK;
}

/**************************************************************************************************************************/

int main(int argc, char** argv) {
    FILE* logf;
    tdns dns;
    char cmd[100];
    char file_dns[100] = "dns.txt";
    char logfile[] = "log.txt";
    int loaded;
    int error;
    int crear;

    /*validacion parámetros de entrada*/
    if(validateInput(argc, argv, (char**)&cmd)!=RES_OK) {
        showHelp(argv[0]);
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
    error = processData(&dns,argv,cmd,logf);
    if(error!=RES_OK) {
        printf("Ocurrió un error: %i.\n",error);
        fclose(logf);
        return RES_ERROR;
    }

    fclose(logf);

    return RES_OK;
}
