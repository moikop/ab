#include "dns.h"
#include "ab.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ARGS_DNS_SEND 6
#define ARGS_DNS_GET_IP 5
#define ARGS_DNS_ADD_DOMAIN 5
#define ARGS_DNS_DELETE_DOMAIN 4
#define DOMAIN_NAME_MAX 256
#define DOMAIN_TAG_MAX 64
#define IP_MAX 16
#define DASH "-"
#define DOT "."
#define MAX_LINE 300
#define FOUNDED 256
#define NOT_FOUNDED 255
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

int orderInsert(TAB *tree, tdomain domain) {
    int error;
    tdomain aux;
    char *buffer = NULL;
    int search = 0;

    search = findDomain(tree,RAIZ,domain.domain);

    if (!search) {
        AB_ElemCte(tree, &aux);
        if (strcasecmp(domain.domain, aux.domain) > 0)
            AB_Insertar(tree, DER, &domain, &error);
        else
            AB_Insertar(tree, IZQ, &domain, &error);
        return RES_OK;
    } else
        return RES_ERROR;
}

void breakDomain(char *domain, TPila *pile) {
    char *pointer = NULL;
    char buffer[DOMAIN_TAG_MAX];

    P_Crear(pile, sizeof(char) * DOMAIN_TAG_MAX);
    pointer = strtok(domain, DOT)
    while (pointer) {
        strcpy(buffer, pointer);
        P_Agregar(pile, buffer);
        pointer = strtok(NULL, DOT);
    }
}

void loadDomain(tdns *dns, TPila *domain) {
    orderInsert(&(dns->ab), domain);
}

void loadTree(tdns *dns, char *configFile) {
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
}

int findDomain(TAB* ab, const int mov, char* domain){
    int *error;
    tdomain Aux;
    AB_MoverCte(ab, mov, error);
    if(*error==FALSE)
        return NOT_FOUNDED;
    AB_ElemCte(ab,&Aux);
    if(strcmp(domain,Aux.domain)==0)
        return FOUNDED;
    if(AB_CanMove(ab,DER))
        return findDomain(ab, DER, domain);
    if(AB_CanMove(ab,IZQ))                            /*VER SI ESTA BIEN HACER UNA PRIMITIVA PARA ESTO EN TDA AB*/
        return findDomain(ab, IZQ, domain);
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

int domainExists(TAB ab, char* domain){
    return findDomain(&ab, RAIZ, domain);
}

/* By Moises */

int addDomain(tdns *dns, char *domain, void *data) {
    char *IP = (char*) data;
    FILE *config;
    char *line;

    line = malloc(sizeof(char) * (strlen(domain) + strlen(IP)));
    if (!line) return 0;

    if (!validateDomain(domain, IP)) return 0;

    strcpy(line, domain);
    strcat(line, " ");
    strcat(line, IP);

    config = fopen("./config/domains", "a");
    if (!config) return 0;

    fprintf(config, "%s\n", line);
    fclose(config);

    free(line);
    return 1;
}

/* By Ignacio */

int addDomain(tdns* dns,char* domain,const tdomain* td) {

    TPila pila_dominio;
    int error;

    breakDomain(domain,&pila_dominio); /*acá adentro crea la pila*/
    addSubDomain(dns->ab,td,pila_dominio,&error);

    return error;
}

int addSubDomain(TAB* a,tdomain* d,TPila pila,int* error) {

    char subdominio[DOMAIN_TAG_MAX];
    tdomain domain;
    tdomain aux;

    /*me fijo si la pila está vacía, si lo está ya se terminó de cargar el dominio o se generó mal la pila*/
    if(P_Vacia(pila)) {
        *error = RES_OK;
        return *error;
    }

    /*saco un elemento de la pila*/
    if(P_Sacar(&pila,subdominio)!=TRUE) {
        *error = RES_ERROR;
        return *error;
    };

    /* si existe en el arbol actual, tomo el corriente, y sigo la búsqueda del siguiente dominio en el árbol del corriente; luego modifico el árbol actual */
    if(domainExists(*a,subdominio)==FOUNDED) { /*lo encontró*/
        if(P_Vacia(pila)) {
            /*la hoja ya existe*/
            *error = RES_ERROR;
            return *error;
        }
        AB_ElemCte(a,&domain);
        addSubDomain(&(domain.subab),d,pila,error);
        if(*error!=RES_OK) return *error;
        AB_ModifCte(a,domain);
        *error = RES_OK;
        return *error;
    }
    else
    {
    /* si no está en el árbol actual el subdominio, puede ser porque se encuentra en una hoja o porque el subdominio todavía no existe pero no es hoja*/
        if(P_Vacia(pila)){
        /* estoy en una hoja , inserto*/
            return orderInsert(a,*d);
        }
        else
        {
        /* todavía estoy entre las ramas*/
            strcpy(aux.domain,subdominio);
            AB_Crear(&(aux.subab),sizeof(tdomain));
            orderInsert(a,aux);
            addSubDomain(&(aux.subab),d,pila,error);
            if(*error!=RES_OK) return *error;
        }
    }
    *error = RES_OK;
    return RES_OK;

}

/***********Funciones de validacion*************/

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

    if (strcmp(argv[1], CMD_SEND)==0 && argc==ARGS_DNS_SEND && validateURL(argv[2])==RES_OK && validateURL(argv[3])==RES_OK) {
        strncpy(*cmd,CMD_SEND,sizeof(CMD_SEND));
    } else if (strcmp(argv[1],CMD_GETIP)==0 && argc==ARGS_DNS_GET_IP && validateURL(argv[2])==RES_OK && validateURL(argv[3])==RES_OK) {
        strncpy(*cmd,CMD_GETIP,sizeof(CMD_GETIP));
    } else if (strcmp(argv[1],CMD_ADDDOMAIN)==0 && argc==ARGS_DNS_ADD_DOMAIN && validateURL(argv[2])==RES_OK && validateIP(argv[3])==RES_OK) {
        strncpy(*cmd,CMD_ADDDOMAIN,sizeof(CMD_ADDDOMAIN));
    } else if (strcmp(argv[1],CMD_DELETEDOMAIN)==0 && argc==ARGS_DNS_DELETE_DOMAIN && validateURL(argv[2])==RES_OK) {
        strncpy(*cmd,CMD_DELETEDOMAIN,sizeof(CMD_DELETEDOMAIN));
    } else {
        return RES_ERROR;
    }
    return RES_OK;
}

/************************************************************/
