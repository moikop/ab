#include "dns.h"
#include "ab.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ARGS_DNS_SEND 6
#define ARGS_DNS_GET_IP 5
#define ARGS_DNS_ADD_DOMAIN 5
#define ARGS_DNS_DELETE_DOMAIN 4
#define DOMAIN_NAME_MAX 255
#define DOMAIN_TAG_MAX 63
#define DASH -
#define DOT .
#define PATTERN_ALL_DOMAINS "^([a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?\.)*[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?$"
#define PATTERN_IP "/([0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])(\.){3}([0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])/"

#define RES_OK 0
#define RES_ERROR 1
#define RES_MEM_ERROR -1


int createDNS(tdns *dns, int dataSize) {
    AB_Crear(&(dns->ab), dataSize);
    dns->dataSize = dataSize;
    return 1;
}

void destroyDNS(tdns *dns) {
    AB_Vaciar(&(dns->ab));
    free(dns);
}

int findDNS(TAB *tree, TPila *domain, char *name, int mov) {
    int res = 0;
    tdomain aux;

    if (AB_Vacio(*tree)) return 0;

    AB_MoverCte(tree, mov);
    AB_ElemCte(*tree, &aux);

    if ((!name) || strcmp(name, "") == 0 )
        P_Sacar(domain, name);

    res = strcasecmp(aux.domain, name);

    if (res < 0) {
        findDNS(tree, domain, name, DER);
    } else if (res > 0) {
        findDNS(tree, domain, name, IZQ);
    } else {
        if (P_Vacia(*domain)) {
            return 1;
        } else {
            P_Sacar(domain, name);
            findDNS(aux.subab, domain, name, RAIZ);
        }
    }

    return 0;
}

int orderInsert(tdns *dns, tdomain domain) {
    int error;
    tdomain aux;
    char *buffer = NULL;
    int search = findDNS(dns, domain.domain, RAIZ);

    if (search < 0) {
        AB_ElemCte(dns->ab, &aux);
        buffer = strtok(domain.domain, ".");
        if (strcasecmp(buffer, aux.domain) > 0)
            AB_Insertar(dns->ab, DER, &domain, &error);
        else
            AB_Insertar(dns->ab, IZQ, &domain, &error);
    } else {
        return 0;
    }
    return 1;
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



void loadTree(tdns *dns, char *configFile) {
    FILE *cfile;
    tdomain temp;
    char line[MAX_LINE];

    cfile = fopen(configFile, "r");

    while(!feof(cfile)) {
        if (fgets(linea, MAX_LINE-1, cfile)) {
            temp.domain = strtok(line, " ");
            tmep.ip = strtok(NULL, "");
            if ((!temp.domain) || (!temp.ip))
                return 0;
            if (validateDomain(temp)
                orderInsert(dns, temp);
            else
                return 0;
        }
    }
    fclose(cfile);
}

int findDomain(tdns dns, const int mov, char* domain){     /*ver el tipo del movimiento*/
    int err, cmp, *error;
    tdomain Aux;
    err=AB_MoverCte(dns->ab, mov, error);
    if(!err)
        return NOT_FOUNDED;
    AB_ElemCte(dns->ab,&Aux);
    if(doamin==Aux.domain)
        return FOUNDED;                                    /*DEFINE*/
    if(AB_CanMove(dns->ab,DER))
        return findDomain(dns, DER, domain);
    if(AB_CanMove(dns->ab,IZQ))                            /*VER SI ESTA BIEN HACER UNA PRIMITIVA PARA ESTO EN TDA AB*/
        return findDomain(dns, IZQ, domain);
}

void getValue(tdns dns, char* domain, void* data){
    tdomain Aux;
    if(AB_Vacio(dns->ab))
        return NO_DATA;              /*DEFINE*/
    AB_ElemCte(dns->ab,&Aux);
    domain=Aux.domain;
    (char*)data=Aux.ip;             /*VER SI SE PUEDE CASTEAR ARGUMENTO Y EL TIPO DE AUX:IP*/
    return RES_OK;
}

int domainExists(tdns dns, char* domain){
    return findDomain(dns, RAIZ, domain);
}

