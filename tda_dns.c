#include "dns.h"
#include "ab.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int createDNS(tdns *dns, int dataSize) {
    dns->ab = (TAB) malloc(sizeof(TAB));
    if (!(dns->ab))
        return -1;
    AB_Crear(dns->ab, dataSize);
    dns->dataSize = dataSize;
    return 1;
}

void destroyDNS(tdns *dns) {
    AB_Vaciar(dns->ab);
    free(dns->ab);
    free(dns);
}

int findDNS(tdns *dns, char *domain, int mov) {
    int cmp, error;
    tdomain aux;
    char *buffer = NULL;

    AB_MoverCte(dns->ab, mov, &error);
    if(error)
        return NOT_FOUNDED;

    AB_ElemCte(dns->ab, &aux);

    if (buffer = strtok(domain, ".ar.")) {
        findDNS(dns->ab, buffer, IZQ);
    } else if (buffer = strtok(domain, ".com.")) {
        findDNS(domain, buffer, DER);
    } else {
        buffer = strrchr(domain, '.');
        if (!buffer)
            return 0;
        cmp = strcasecmp(buffer, aux.domain);
        if (cmp < 0) {
            strncpy(buffer, domain, strlen(domain) - strlen(buffer));
            findDNS(dns, buffer, IZQ);
        } else if (cmp > 0) {
            strncpy(buffer, domain, strlen(domain) - strlen(buffer));
            findDNS(dns, buffer, DER);
        } else {
            return 1;
        }
    }
    return 0;
}


int orderInsert(tdns *dns, tdomain domain);

int validateIP(int ip) {
    if ((ip > 0) && (ip <= 255))
        return 1;
    return 0;
}

int validateDomain(tdomain domain) {
    tip temp_ip;
    int i = 0;

    temp_ip.first = strtol(strtok(domain.ip, "."), NULL, 10);
    temp_ip.second = strtol(strtok(NULL, "."), NULL, 10);
    temp_ip.third = strtol(strtok(NULL, "."), NULL, 10);
    temp_ip.fourth = strtol(strtok(NULL, "."), NULL, 10);

    if ((!validateIP(temp_ip.first)) || (!validateIP(temp_ip.second)) || (!validateIP(temp_ip.third)) || (!validateIP(temp_ip.fourth)))
        return 0;

    for (i = 0; i < strlen(domain.domain); i++) {
        if ((isalpha(domain.domain[i]) || (strcmp(domain.domain[i], ".") == 0))
            continue;
        else
            return 0;
    }

    return 1;
}

void loadTree(tdns *dns, char *configFile) {
    FILE *cfile;
    tdomain temp;
    char *line[MAX_LINE];

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

int AB_Busqueda(tdns dns, const int mov, char* domain){     /*ver el tipo del movimiento*/
    int err, cmp, *error;
    tdomain Aux;
    err=AB_MoverCte(dns->ab, mov, error);
    if(!err)
        return NOT_FOUNDED;
    AB_ElemCte(dns->ab,&Aux);
    if(doamin==Aux.domain)
        return FOUNDED;                                    /*DEFINE*/
    if(AB_CanMove(dns->ab,DER))
        return AB_Busqueda(dns, DER, domain);
    if(AB_CanMove(dns->ab,IZQ))                            /*VER SI ESTA BIEN HACER UNA PRIMITIVA PARA ESTO EN TDA AB*/
        return AB_Busqueda(dns, IZQ, domain);
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
    return AB_Busqueda(dns, raiz, domain);
}

