#include "dns.h"

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
