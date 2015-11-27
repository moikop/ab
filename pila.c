#include "Pila.h"
#include <malloc.h>
#include <memory.h>

void P_Crear(TPila *pP, int TamanioDato)
{
    pP->Tope = NULL;
    pP->TamanioDato = TamanioDato;
}

void P_Vaciar(TPila *pP)
{
    TNodoPila *pAux = pP->Tope;
    TNodoPila *pSig;
    while (pAux)
    {
        pSig = pAux->Siguiente;
        free(pAux->Elem);
        free(pAux);
        pAux = pSig;
    }
    pP->Tope= NULL;
}

int P_Vacia(TPila P)
{
    return (P.Tope==NULL);
}

int P_Agregar(TPila *pP, void* pE)
{
    TNodoPila *pNodo = (TNodoPila*) malloc(sizeof(TNodoPila));
    if (!pNodo)
        return FALSE;
    else
    {
        pNodo->Siguiente = pP->Tope;
        pP->Tope = pNodo;
        pNodo->Elem = malloc (pP->TamanioDato);
        memcpy(pNodo->Elem, pE, pP->TamanioDato);
        return TRUE;
    }
}

int P_Sacar(TPila *pP, void* pE)
{
    TNodoPila *pAux = pP->Tope;
    pP->Tope = pP->Tope->Siguiente;
    memcpy(pE, pAux->Elem, pP->TamanioDato);
    free(pAux->Elem);
    free(pAux);
    return TRUE;
}
