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
#define RES_HOJA_BORRADA 200

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
int AB_Borrar_Hoja(TAB *a);
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

    breakDomain(url,&pila);
    P_Sacar(&pila,domain);
    return findDNS(&(dns.ab),&pila,domain,RAIZ);
}

void deleteDomain(tdns *dns, char* url) {

    char domain[DOMAIN_TAG_MAX];
    TPila pila;

    printf("deleteDomain: Entré en deleteDomain.\n");
    breakDomain(url,&pila);
    P_Sacar(&pila,domain);
    if(deleteData(&(dns->ab),&pila,domain,RAIZ)!=RES_OK) {
        printf("No se pudo borrar el dato.\n");
    }

}

/************************************ Funciones utilizadas ************************************************************/

int findDNS(TAB *tree, TPila *url,char* domain, int mov) {

	tdomain tdaux;

    /* si existe en el arbol actual, tomo el corriente, y sigo la búsqueda del siguiente dominio en el árbol del corriente*/
    if(findDomain(tree,RAIZ,domain)==RES_OK) {
        if(P_Vacia(*url)) return RES_OK;
        printf("findDNS: Existe el subdominio = %s.\n",domain);
        printf("findDNS: como existe este subdominio paso al siguiente nivel del arbol.\n");
        AB_ElemCte(*tree,&tdaux);
        P_Sacar(url,domain);
        printf("findDNS: Siguiente subdominio = %s.\n",domain);
        return findDNS(&(tdaux.subab),url,domain,RAIZ);
    } else
        printf("findDNS: no existe el subdomino, no existe la url.\n");
    return RES_ERROR;
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
    tdomain aux;

    printf("deleteData: entre en deleteData\n");

    if(AB_Vacio(*tree)) return RES_ERROR; /* arbol vacio, no lo encontré*/

    AB_MoverCte(tree, mov,&error);
    printf("deleteData: muevo el corriente.\n");
    if(error) return RES_ERROR; /* no lo encontre */

    AB_ElemCte(*tree, &aux);
    printf("deleteData: el elem del corriente.\n");

    res = strcasecmp(aux.domain,domain);

    if (res < 0) {
        printf("deleteData: me muevo para la derecha.\n");
        return deleteData(tree,url,domain,DER);
    } else if (res > 0) {
        printf("deleteData: me muevo para la izquierda.\n");
        return deleteData(tree,url,domain,IZQ);
    } else {
        printf("deleteData: encontré un match para el subdominio");
        if (P_Vacia(*url)) { /** estoy en la hoja que tengo que borrar**/
            printf("deleteData: la pila esta vacia.\n");
            if(!AB_Vacio(aux.subab)) {
                printf("deleteData: el arbol en el elem no esta vacio, entonces no es hoja.\n");
                return RES_ERROR;
            }
            printf("deleteData: el arbol en el elem esta vacio, entonces es hoja.\n");
            if(AB_Borrar_Hoja(tree)!=RES_OK) {
                printf("deleteData: no se pudo borar la hoja.\n");
                return RES_ERROR;
            }
            AB_ElemCte(*tree, &aux);
            printf("deleteData: el corriente es = %s.\n",aux.domain);
            printf("deleteData: Se borro el dominio.\n");
            return RES_OK; /*lo encontramos*/
        } else {
            P_Sacar(url,domain);
            printf("deleteData: sigo buscando.\n");
            return deleteData(&(aux.subab),url,domain,RAIZ);
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
            printf("addSubDomain:el subdominio agregado es %s.\n",d->domain);
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
