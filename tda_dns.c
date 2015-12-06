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
    char *name = malloc(sizeof(char) * 64);
    int search = 0;

    if (!name) return 0;

    search = findDNS(tree, domain.domain, name, RAIZ);
    free(name);

    if (!search) {
        AB_ElemCte(tree, &aux);
        buffer = strtok(domain.domain, ".");
        if (strcasecmp(buffer, aux.domain) > 0)
            AB_Insertar(tree, DER, &domain, &error);
        else
            AB_Insertar(tree, IZQ, &domain, &error);
    } else {
        return 0;
    }
    return 1;
}

void breakDomain(char *domain, TPila *pile) {
    char *pointer = NULL;
    char buffer[64];

    P_Crear(pile, sizeof(char) * 64);
    pointer = strtok(domain, ".")

    while (pointer) {
        strcpy(buffer, pointer);
        P_Agregar(pile, buffer);
        pointer = strtok(NULL, ".");
    }
}

void loadDomain(tdns *dns, TPila *domain) {
    orderInsert(dns->ab, domain);
}

void loadTree(tdns *dns, char *configFile) {
    FILE *cfile;
    char line[MAX_LINE];
    char IP[14];
    char url[64];
    char *buffer;
    TPila domain;

    cfile = fopen(configFile, "r");
    if(!cfile) return RES_ERROR;

    while(!feof(cfile)) {
        if (fgets(line, MAX_LINE-1, cfile)) {
            buffer = strtok(line, " ");
            strncpy(url, buffer, 64);
            buffer = strtok(NULL, " ");
            strncpy(IP, buffer, 14);
            if (validateInput(url, IP)) {
                breakDomain(url, &domain);
                loadDomain(dns, &domain);
            }
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
    if(strcmp(domain,Aux.domain)==0)
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

int agregarDominio(tdns* dns,tdomain* td) {

    TPila pila_dominio;
    int error;
    /*TODO parsear tdomain.dominio y guardarlo en la pila*/
    /*ahora con el dominio en la pila, voy a agregando los subdominios*/

    /*TODO agregarSubDominio(TABO* a,tdomain* d,TPila pila,int* error)*/
    return error;
}

int agregarSubDominio(TAB* a,tdomain* d,TPila pila,int* error) {

    char* subdominio;
    tdomain domain;
    if(P_Vacia(pila)) {*error = RES_OK;return *error}

    subdominio = (char*)malloc(sizeof(char)*DOMAIN_TAG_MAX+1);
    if(!subdominio) return RES_MEM_ERROR;

    if(P_Sacar(&pila,subdominio) && P_Vacia(pila)) {
    /*ultima iteración, estamos en la hoja*/
    /*hay que inicializar un tdomain con : el nombre del subdominio, la ip correspondiente a la url completa, un árbol vacío, y el diccionario de encriptación*/
        echo "ultima iteración, estamos en la hoja\n";
        ABO_Insertar(a,&domain,error);
        return *error;
    }

    /*a esta altura ya saqué un elemento de la pila*/

    /*hay que inicializar un tdomain con: el nombre del subdominio, sin ip, crearle un árbol, y sin diccionario de encriptación*/

    echo "en plena iteración\n";

    ABO_Insertar(a,&domain,error);

    return *error;
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

/*
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
*/
/************************************************************/
