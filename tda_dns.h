#ifndef
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
        char ip[12];
        TAB *subab;
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
    int createDNS(tdns *dns, int dataSize);

    /*
    destruirDNS
    Descripcion: Destruye una instancia de TDA DNS.
    Precondiciones: dns creado y vacio.
    Postcondiciones: Se destruyó la instancia dns.
    */
    void destroyDNS(tdns *dns);

    /*
     Descripcion: Agrega un dominio con un dato asociado.
    Precondiciones: dns creado. dominio es un nombre de dominio válido y no existente
    en dns. dato apunta a una variable de tipo T.
    Postcondiciones: Se agregó el domino dominio, y se asocio
    el valor dato.
    */
    int addDomain(tdns *dns, char* domain, void* data);

    /*
    obtenerValor
    Descripcion: Obtiene el valor asociado a un dominio.
    Precondiciones: dns creado. dominio es un nombre de dominio válido existente en
    dns. dato apunta a una variable de tipo T.
    Postcondiciones: Se copió a dato el valor asociado al dominio dominio.
    */
    void getValue(tdns dns, char* domain, void* data);

    /*
    existeDominio
    Descripcion: Devuelve TRUE si dominio existe en dns. En cualquier otro caso devuelve
    FALSE.
    Precondiciones: dns creado. dominio es un nombre de dominio válido.
    Postcondiciones: No realiza modificaciones.
    */
    int domainExists(tdns dns, char* domain);

    /*
    eliminarDominio
    Descripcion: Elimina un dominio.
    Precondiciones: dns creado. dominio es un nombre de dominio válido y existente en
    dns.
    Postcondiciones: Se eliminó dominio, y su valor asociado, de dns
    */
    void deleteDomain(tdns *dns, char* domain);


#endif
