#ifndef __AB_H__
    #define __AB_H__

    #include <stdlib.h>

    #define IZQ 1
    #define DER 2
    #define PAD 3
    #define RAIZ 4

    typedef struct TNodoAB
    {
    void* elem;
    struct TNodoAB *izq, *der;
    } TNodoAB;

    typedef struct TAB
    {
    TNodoAB *raiz,*cte;
    int tamdato;
    } TAB;

    void AB_Crear(TAB *a,int tdato);

    void AB_ElemCte(TAB a,void *elem);

    void AB_ModifCte(TAB *a,void *elem);

    void AB_MoverCte(TAB *a,int mov,int *error);

    void AB_Vaciar(TAB *a);

    int AB_Vacio(TAB a);

    void AB_Insertar(TAB *a,int mov,void *elem,int *error);

#endif

