#ifndef __DNS_H__
    #define __DNS_H__

    #if !defined(NULL)
        #define NULL 0
    #endif

    #if !defined(FALSE)
        #define FALSE 0
    #endif

    #if !defined(TRUE)
        #define TRUE 1
    #endif

    #include "ab.h"

    #define MAX_LINE 270

    typedef struct {
        char domain[256];
        char ip[16];
        TAB subab;
        char offset;
    }tdomain;

    typedef struct {
        int dataSize;
        TAB ab;
    }tdns;

    typedef struct {
        int first;
        int second;
        int third;
        int fourth;
    }tip;

    /*
    crearDNS
    Descripcion : Crea una instancia de TDA DNS.
    Precondiciones : dns tiene sizeof(TDADNS) bytes reservados. dns no creado. tamDato
    es el tamaño del tipo de dato T que tendrán los elementos que se van a guardar
    asociados a un dominio.
    Postcondiciones : Se inicializa la estructura apuntada por dns.
    */

    int createDNS(tdns *dns, int dataSize);

    /*
    destruirDNS
    Descripcion: Destruye una instancia de TDA DNS.
    Precondiciones: dns creado y vacio.
    Postcondiciones: Se destruyó la instancia dns.
    */
    void destroyDNS(tdns *dns);

    /*
     Descripcion: Agrega una url con un dato asociado.
    Precondiciones: dns creado. url es un nombre de url válido y no existente
    en dns. dato apunta a una variable de tipo T.
    Postcondiciones: Se agregó la url, y se asocio
    el valor dato.
    */
    int addDomain(tdns *dns, char* url, void* data);

    /*
    obtenerValor
    Descripcion: Obtiene el valor asociado a una url.
    Precondiciones: dns creado. url es un nombre de url válido existente en
    dns. dato apunta a una variable de tipo T.
    Postcondiciones: Se copió a dato el valor asociado a la url.
    */
    void getValue(tdns dns, char* url, void* data);

    /*
    existeUrl
    Descripcion: Devuelve TRUE si url existe en dns. En cualquier otro caso devuelve
    FALSE.
    Precondiciones: dns creado. url es un nombre de url válido.
    Postcondiciones: No realiza modificaciones.
    */
    int urlExists(tdns dns, char* url);

    /*
    eliminarUrl
    Descripcion: Elimina un url.
    Precondiciones: dns creado. url es un nombre de url válido y existente en
    dns.
    Postcondiciones: Se eliminó url, y su valor asociado, de dns
    */
    void deleteUrl(tdns *dns, char* url);


#endif
