#include "tda_dns.h"
#include "ab.h"
#include "pila.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

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


/******************************* Declaraciones de funciones *****************************************/


int findDNS(TAB *tree, TPila *url,char* domain, int mov);
void tdomainCopy(tdomain *dst, tdomain *src);
int getData(TAB *tree, TPila *url,char* domain, int mov,tdomain* td);
int deleteData(TAB* tree,TPila* url,char* domain,int mov);
void breakDomain(char *domain, TPila *pile);
void breakDomain(char *domain, TPila *pile);
int domainExists(TAB a,char* domain);
int addSubDomain(TAB* a,const tdomain* d,TPila pila);
int findDomain(TAB* ab, const int mov, char* domain);
int searchPlace(TAB* ab,tdomain* domain,int* mov);
int orderInsert(TAB *tree, tdomain domain);

/******************************************************************************************************/

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

int addDomain(tdns* dns,char* url,const tdomain* td) {

    TPila pila_dominio;
    printf("addDomain: inicia la insercion.\n");
    breakDomain(url,&pila_dominio); /*acá adentro crea la pila*/
    return addSubDomain(&(dns->ab),td,pila_dominio);
}

void getValue(tdns* dns, char* url, tdomain* td){

    char domain[DOMAIN_TAG_MAX];
    TPila pila;

    if(AB_Vacio(dns->ab))
        return;

    breakDomain(url,&pila);
    P_Sacar(&pila,domain);
    if(getData(&(dns->ab),&pila,domain,RAIZ,td)!=RES_OK) {
        printf("No se pudo obtener el dato.\n");
    }

}

int urlExists(tdns dns, char* url){
    TPila pila;
    char domain[DOMAIN_TAG_MAX];

    strcpy(domain, "");

    breakDomain(url,&pila);
    return findDNS(&(dns.ab),&pila,domain,RAIZ);
}

void deleteUrl(tdns *dns, char* url) {

    char domain[DOMAIN_TAG_MAX];
    TPila pila;

    breakDomain(url,&pila);
    if(deleteData(&(dns->ab),&pila,domain,RAIZ)!=RES_OK) {
        printf("No se pudo obtener el dato.\n");
    }

}

/************************************ Funciones utilizadas ************************************************************/

int findDNS(TAB *tree, TPila *url,char* domain, int mov) {
    int res;
    int error;
    tdomain aux;

    if(AB_Vacio(*tree)) return RES_ERROR; /* arbol vacio, no lo encontré*/

    AB_MoverCte(tree, mov,&error);
    if(error) return RES_ERROR; /* no lo encontre */

    AB_ElemCte(*tree, &aux);

   if ((!domain) || strcmp(domain, "") == 0 )
        P_Sacar(url,domain);

    /*res = strcasecmp(aux.domain,domain);*/
    res = strcmp(aux.domain,domain);

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

void tdomainCopy(tdomain *dst, tdomain *src) {
    if (src->domain)
        strcpy(dst->domain, src->domain);
    if (src->ip)
        strcpy(dst->ip, src->ip);
    if (&(src->subab)!=NULL)
        memcpy(&(dst->subab),&(src->subab), sizeof(TAB));
}

int getData(TAB *tree, TPila *url,char* domain, int mov,tdomain* td) {
    int res;
    int error;
    tdomain aux;

    printf("Entré en getData.\n");
    if(AB_Vacio(*tree)) return RES_ERROR; /* arbol vacio, no lo encontré*/

    AB_MoverCte(tree, mov,&error);
    if(error!=RES_OK) {
        printf("No puedo mover el corriente.\n");
        return RES_ERROR; /* no lo encontre */
    }

    AB_ElemCte(*tree, &aux);
    printf("Obtuve este corriente: %s.\n",aux.domain);

    res = strcmp(aux.domain,domain);
    printf("La comparación dio: %s %s %i\n",aux.domain,domain,res);

    if (res < 0) {
        printf("Voy a la derecha.\n");
        return getData(tree,url,domain,DER,td);
    } else if (res > 0) {
        printf("Voy a la izquierda.\n");
        return getData(tree,url,domain,IZQ,td);
    } else {
        if (P_Vacia(*url)) {
            tdomainCopy(td,&aux);
            printf("Encontré el nodo %s.\n",td->domain);
            return RES_OK; /*lo encontramos*/
        } else {
            P_Sacar(url,domain);
            printf("Sigo iterando: %s\n",domain);
            return getData(&(aux.subab),url,domain,RAIZ,td);
        }
    }
}

int deleteData(TAB* tree,TPila* url,char* domain,int mov) {

    int res;
    int error;
    /*char domain[DOMAIN_TAG_MAX];*/
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
        return getData(tree,url,domain,DER,&aux);
    } else if (res > 0) {
        return getData(tree,url,domain,IZQ,&aux);
    } else {
        if (P_Vacia(*url)) {
            strcpy(reemplazo.domain,"");
            strcpy(reemplazo.ip,"");
            reemplazo.offset = 0;
            AB_Vaciar(&(reemplazo.subab));
            AB_ModifCte(tree,&reemplazo);
            return RES_OK; /*lo encontramos*/
        } else {
            P_Sacar(url,domain);
            return getData(&(aux.subab),url,domain,RAIZ,&aux);
        }
    }
}

void breakDomain(char *domain, TPila *pile) {
    char *pointer = NULL;
    char buffer[DOMAIN_TAG_MAX];
    char* url_aux;

    url_aux = (char*)malloc(sizeof(char)*strlen(domain)+1);
    if(!url_aux) printf("breakDomain: no hay recursos para variable auxiliar.\n");

    strcpy(url_aux,domain);

    P_Crear(pile, sizeof(char) * DOMAIN_TAG_MAX);
    pointer = strtok(url_aux, DOT);
    printf("breakDomain: url = %s\n",url_aux);
    while (pointer) {
        printf("breakDomain: subdominio = %s\n",pointer);
        strcpy(buffer, pointer);
        P_Agregar(pile, buffer);
        pointer = strtok(NULL, DOT);
    }
}

int domainExists(TAB a,char* domain) {
    return findDomain(&a,RAIZ,domain);
}

int addSubDomain(TAB* a,const tdomain* d,TPila pila) {

    char subdominio[DOMAIN_TAG_MAX];
    tdomain domain;
    tdomain aux;
    int inser;

    /*me fijo si la pila está vacía, si lo está ya se terminó de cargar el dominio o se generó mal la pila*/
    if(P_Vacia(pila)) {
        printf("addSubDomain: Pila vacia.\n");
        return RES_OK;
    }

    /*saco un elemento de la pila*/
    if(P_Sacar(&pila,subdominio)!=TRUE) {
        printf("addSubDomain: No se pudo sacar un elemento de la pila.\n");
        return RES_ERROR;
    };

    /* si existe en el arbol actual, tomo el corriente, y sigo la búsqueda del siguiente dominio en el árbol del corriente; luego modifico el árbol actual */
    if(domainExists(*a,subdominio)==RES_OK) { /*lo encontró*/
        printf("addSubDomain: Existe el subdominio.\n");
        if(P_Vacia(pila)) {
            printf("addSubDomain: ya existe la hoja.\n");
            /*la hoja ya existe*/
            return RES_ERROR;
        }
        AB_ElemCte(*a,&domain);
        printf("addSubDomain: como existe este subdominio paso al siguiente nivel del arbol.\n");
        if(addSubDomain(&(domain.subab),d,pila)!=RES_OK) return RES_ERROR;
        AB_ModifCte(a,&domain);
        return RES_OK;
    }
    else
    {
        printf("addSubDomain: No existe el subdominio.\n");
        /* si no está en el árbol actual el subdominio, puede ser porque se encuentra en una hoja o porque el subdominio todavía no existe pero no es hoja*/
        if(P_Vacia(pila)){
            /* estoy en una hoja , inserto*/
            inser = orderInsert(a,*d);
            printf("addSubDomain: Estoy en la hoja, ya insertado: %i.\n",inser);
            printf("addSubDomain: %s\n",d->domain);
            return inser;
        }
        else
        {
            printf("addSubDomain: Tengo que insertar un nuevo subdominio para seguir.\n");
            /* todavía estoy entre las ramas*/
            strcpy(aux.domain,subdominio);
            AB_Crear(&(aux.subab),sizeof(tdomain));
            orderInsert(a,aux);
            AB_ElemCte(*a,&domain);
            printf("addSubDomain: %s\n",domain.domain);
            printf("addSubDomain: paso al siguiente nivel del arbol.\n");
            if(addSubDomain(&(domain.subab),d,pila)!=RES_OK) return RES_ERROR;
            AB_ModifCte(a,&domain);
            printf("addSubDomain: Actualice correctamente.\n");
            return RES_OK;
        }
    }
}

int findDomain(TAB* ab, const int mov, char* domain){
    int error;
    tdomain Aux;
    int cmp;

    AB_MoverCte(ab, mov, &error);

    if(error==RES_ERROR)
        return RES_ERROR;

    AB_ElemCte(*ab,&Aux);

    cmp = strcmp(domain,Aux.domain);
    if(cmp==0) return RES_OK;

    if(cmp<0) return findDomain(ab,IZQ,domain);
    else return findDomain(ab,DER,domain);
}

int searchPlace(TAB* ab,tdomain* domain,int* mov) {

    tdomain tdaux;
    int cmp;
    int error;

    AB_MoverCte(ab,*mov,&error);
    if(error==RES_ERROR) return RES_ERROR;
    AB_ElemCte(*ab,&tdaux);
    cmp = strcmp(domain->domain,tdaux.domain);
    if(cmp==0) return RES_OK;
    if(cmp<0) *mov=IZQ;
    else *mov=DER;
    return searchPlace(ab,domain,mov);
}

int orderInsert(TAB *tree, tdomain domain) {
    int error;
    int search = 0;
    int mov = RAIZ ;

    search = searchPlace(tree,&domain,&mov);

    if(search==RES_OK) return RES_ERROR;

    AB_Insertar(tree,mov,&domain,&error);

    return error;
}
