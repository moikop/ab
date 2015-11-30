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

int orderInsert(tdns *dns, tdomain *domain);

void loadTree(tdns *dns, char *configFile) {
    FILE *cfile;
    tdomain temp;
    char *line[MAX_LINE];

    cfile = fopen(configFile, "r");

    while(!feof(cfile)) {
        if (fgets(linea, MAX_LINE-1, cfile)) {
            temp.domain = strtok(line, " ");
            tmep.ip = strtok(NULL, "");
            if ((!temp.domain) || (!temp.ip))
                return 0;
            orderInsert(dns, temp);
        }
    }
    fclose(cfile);
}
