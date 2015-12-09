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
#define NO_DATA 254
#define RES_OK 0
#define RES_ERROR 1
#define RES_MEM_ERROR -1

/******************************************** Implementacion de primitivas **********************************************/

int createDNS(tdns *dns, int dataSize) {
    AB_Crear(&(dns->ab), dataSize);
    dns->dataSize = dataSize;
    return RES_OK;
}

void destroyDNS(tdns *dns) {
    AB_Vaciar(&(dns->ab));
    free(dns);
}

/* toma una estructura tdomain completa (elem de la hoja) y la url*/
/* hace pila con la url, y llama a addSubDomain*/
int addDomain(tdns* dns,char* url,const tdomain* td) {

    TPila pila_dominio;
    int error;

    breakDomain(url,&pila_dominio); /*acá adentro crea la pila*/
    addSubDomain(dns->ab,td,pila_dominio,&error);

    return error;
}

void getValue(tdns* dns, char* url, tdomain* td){

    char domain[DOMAIN_TAG_MAX];
    TPila pila;
    tdomain Aux;

    if(AB_Vacio(dns->ab))
        return NO_DATA;

    breakDomain(url,&pila);
    if(getData(dns->ab,&pila,domain,RAIZ,td)!=RES_OK) {
        printf("No se pudo obtener el dato.\n");
    }

}

int urlExists(tdns dns, char* url){

    TPila pila;
    char* domain[DOMAIN_TAG_MAX] = "";

    breakDomain(url,&pila);
    return findDNS(&(dns.ab),&pila,domain,RAIZ);
}

void deleteUrl(tdns *dns, char* url) {

    char domain[DOMAIN_TAG_MAX];
    TPila pila;
    tdomain Aux;

    breakDomain(url,&pila);
    if(deleteData(dns->ab,&pila,domain,RAIZ)!=RES_OK) {
        printf("No se pudo obtener el dato.\n");
    }

}

/************************************ Funciones utilizadas ************************************************************/

int findDNS(TAB *tree, TPila *url,char* domain, int mov) {
    int res;
    int error;
    char domain[DOMAIN_TAG_MAX];
    tdomain aux;

    if(AB_Vacio(*tree)) return RES_ERROR; /* arbol vacio, no lo encontré*/

    AB_MoverCte(tree, mov,&error);
    if(error) return RES_ERROR; /* no lo encontre */

    AB_ElemCte(*tree, &aux);

   if ((!domain) || strcmp(domain, "") == 0 )
        P_Sacar(url,domain);

    res = strcasecmp(aux.domain,domain);

    if (res < 0) {
        return findDNS(tree,url,domain,DER);
    } else if (res > 0) {
        return findDNS(tree,url,domain,IZQ);
    } else {
        if (P_Vacia(*url)) {
            return RES_OK; /*lo encontramos*/
        } else {
            P_Sacar(url,domain);
            return findDNS(&(aux.subab),url,domain,RAIZ);
        }
    }
}

int getData(TAB *tree, TPila *url,char* domain, int mov,tdomain* td) {
    int res;
    int error;
    char domain[DOMAIN_TAG_MAX];
    tdomain aux;

    if(AB_Vacio(*tree)) return RES_ERROR; /* arbol vacio, no lo encontré*/

    AB_MoverCte(tree, mov,&error);
    if(error) return RES_ERROR; /* no lo encontre */

    AB_ElemCte(*tree, &aux);

   if ((!domain) || strcmp(domain, "") == 0 )
        P_Sacar(url,domain);

    res = strcasecmp(aux.domain,domain);

    if (res < 0) {
        return getData(tree,url,domain,DER,td);
    } else if (res > 0) {
        return getData(tree,url,domain,IZQ,td);
    } else {
        if (P_Vacia(*url)) {
            AB_Copy(td,&aux);
            return RES_OK; /*lo encontramos*/
        } else {
            P_Sacar(url,domain);
            return getData(&(aux.subab),url,domain,RAIZ,td);
        }
    }
}

int deleteData(TAB* ab,TPila* pila,char* domain,int mov) {

    int res;
    int error;
    char domain[DOMAIN_TAG_MAX];
    tdomain aux;
    tdomain reemplazo;

    if(AB_Vacio(*tree)) return RES_ERROR; /* arbol vacio, no lo encontré*/

    AB_MoverCte(tree, mov,&error);
    if(error) return RES_ERROR; /* no lo encontre */

    AB_ElemCte(*tree, &aux);

   if ((!domain) || strcmp(domain, "") == 0 )
        P_Sacar(url,domain);

    res = strcasecmp(aux.domain,domain);

    if (res < 0) {
        return getData(tree,url,domain,DER,td);
    } else if (res > 0) {
        return getData(tree,url,domain,IZQ,td);
    } else {
        if (P_Vacia(*url)) {
            strcpy(reemplazo.domain,"");
            strcpy(reemplazo.ip,"");
            strcpy(reemplazo.offset,"");
            AB_Vaciar(&(reemplazo.subab));
            AB_ModifCte(ab,&reemplazo);
            return RES_OK; /*lo encontramos*/
        } else {
            P_Sacar(url,domain);
            return getData(&(aux.subab),url,domain,RAIZ,td);
        }
    }
}


int orderInsert(TAB *tree, tdomain domain) {
    int error;
    tdomain aux;
    char *buffer = NULL;
    int search = 0;

    search = findDomain(tree,RAIZ,domain.domain);

    if (!search) {
        AB_ElemCte(*tree, &aux);
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

int findDomain(TAB* ab, const int mov, char* domain){
    int *error;
    tdomain Aux;
    AB_MoverCte(ab, mov, error);
    if(*error==FALSE)
        return NOT_FOUNDED;
    AB_ElemCte(*ab,&Aux);
    if(strcmp(domain,Aux.domain)==0)
        return FOUNDED;
    if(AB_CanMove(*ab,DER))
        return findDomain(ab, DER, domain);
    if(AB_CanMove(*ab,IZQ))                            /*VER SI ESTA BIEN HACER UNA PRIMITIVA PARA ESTO EN TDA AB*/
        return findDomain(ab, IZQ, domain);
}

/* recibe una referencia a un árbol, el dato d, la pila donde está la url, y una variable error*/
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
