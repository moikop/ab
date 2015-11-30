#include "dns.h"

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

