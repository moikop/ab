#include "ab.h"
#include <memory.h>

void AB_Crear(TAB *a,int tdato)
{
    a->tamdato = tdato;
    a->raiz = NULL;
    a->cte = NULL;
}

void AB_ElemCte(TAB a,void *elem)
{
    memcpy(elem,a.cte->elem,a.tamdato);
}

void AB_ModifCte(TAB *a,void *elem)
{
    memcpy(a->cte->elem,elem,a->tamdato);
}

TNodoAB* BuscarPadre(TNodoAB *padre,TNodoAB *hijo)     /*FUNCION INTERNA*/
{
    TNodoAB *paux = NULL;
    if ((padre->izq == hijo) || (padre->der == hijo))
        return padre;
    else
    {
        if (padre->izq != NULL)
            paux = BuscarPadre(padre->izq,hijo);
        if ((padre->der != NULL) && (paux == NULL))
            paux = BuscarPadre(padre->der,hijo);
        return paux;
    }
}

void AB_MoverCte(TAB *a,int mov,int *error)
{
    *error = 0;
    switch (mov)
    {
        case IZQ : if (a->cte->izq != NULL)
                        a->cte = a->cte->izq;
                    else
                        *error = 1;
                    break;

        case DER : if (a->cte->der != NULL)
                        a->cte = a->cte->der;
                    else
                        *error = 1;
                    break;

        case PAD : if (a->cte != a->raiz)
                        a->cte = BuscarPadre(a->raiz,a->cte);
                    else
                        *error = 1;
                    break;

        case RAIZ : if (a->raiz != NULL)
                        a->cte = a->raiz;
                    else
                        *error = 1;
                    break;

        default : *error = 1;

    }
}

void VaciarSub(TNodoAB *pnodo)
{
    if (pnodo != NULL)
    {
        if ((pnodo->izq == NULL) && (pnodo->der == NULL))
        {
            free(pnodo->elem);
            free(pnodo);
        }
        else
        {
            VaciarSub(pnodo->izq);
            VaciarSub(pnodo->der);
            free(pnodo->elem);
            free(pnodo);
        }
    }
}

void AB_Vaciar(TAB *a)
{
    VaciarSub(a->raiz);
    a->raiz = NULL;
    a->cte = NULL;
}

void AB_Insertar(TAB *a,int mov,void *elem,int *error)
{
    TNodoAB *paux;
    *error = 0;
    paux = (TNodoAB*) malloc(sizeof(TNodoAB));
    paux->izq = NULL;
    paux->der = NULL;
    if (paux)
    {
        paux->elem = malloc(a->tamdato);
        if (paux->elem)
        {
            memcpy(paux->elem,elem,a->tamdato);
            switch(mov)
            {

                case IZQ : if (a->cte->izq == NULL)
                                a->cte->izq = paux;
                            else
                                *error = 1;
                            break;

                case DER : if (a->cte->der == NULL)
                                a->cte->der = paux;
                            else
                                *error = 1;
                            break;

                case RAIZ : if (a->raiz == NULL)
                                a->raiz = paux;
                            else
                                *error = 1;
                            break;

                default : *error = 1;

            }
            if (*error)
            {
                free(paux->elem);
                free(paux);
            }
            else
                a->cte = paux;
        }
        else /* if (paux->elem) */
        {
            *error = 1;
            free(paux);
        }
    }
    else /* if (paux) */
        *error = 1;
}

int AB_Vacio(TAB a)
{
    if (a.raiz == NULL)
        return 1;
    else
        return 0;
}
