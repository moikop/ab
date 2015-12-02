#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define ARGS_DNS_SEND 6
#define ARGS_DNS_GET_IP 5
#define ARGS_DNS_ADD_DOMAIN 5
#define ARGS_DNS_DELETE_DOMAIN 4
#define DOMAIN_NAME_MAX 255
#define DOMAIN_TAG_MAX 63
#define HYPHEN -
#define DOT .
#define PATTERN_ALL_DOMAINS "^([a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?\.)*[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?$"
#define PATTERN_IP "/([0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])(\.){3}([0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])/"

#define RES_OK 0
#define RES_ERROR 1
#define RES_MEM_ERROR -1

int match_pattern(const char *string, char *pattern)
{
    int status;
    regex_t re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0)
        return RES_ERROR;

    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return RES_ERROR;
    }
    return RES_OK;
}

int validateInput(int argc, char* argv) {

    if (strcmp(argv[1],CMD_SEND)==0 && argc==ARGS_DNS_SEND) {
        return (validateUrl(argv[2]) || validateUrl(argv[3]) || initializeLog(argv[5]));
    } else if(strcmp(argv[1],CMD_GETIP)==0 && argc==ARGS_DNS_GET_IP) {
        return (validateUrl(argv[2]) || validateUrl(argv[3]) || initializeLog(argv[4]));
    } else if(strcmp(argv[1],CMD_ADDDOMAIN)==0 && argv==ARGS_DNS_ADD_DOMAIN) {
        return (validateUrl(argv[2]) || validateIp(argv[3]) || initializeLog(argv[4]));
    } else if(strcmp(argv[1],CMD_DELETEDOMAIN)==0 && argc==ARGS_DNS_DELETE_DOMAIN) {
        return (validateUrl(argv[2]) || initializeLog(argv[3]));
    } else return RES_ERROR;
}

int validateIp(char* ip) {
    return match_pattern(ip,PATTERN_IP);
}

int validateUrl(char* url){
    if(strlen(url)>DOMAIN_NAME_MAX) return RES_ERROR;
    return match_pattern(url,PATTERN_ALL_DOMAINS);
}

int main (int argc,char* argv) {

    char ip[15] = "255.255.255.255";
    char url[255] = "www.google.com.ar";

    if(validateIp(ip)==RES_OK) printf("Es IP.\n");
    else printf("No es IP.");

    if(validateUrl(url)==RES_OK) printf("Es URL.\n");
    else printf("No es URL.");

    if(validateInput(int argc, char* argv) == RES_OK) printf("Argumentos Validos.\n");
    else printf("Argumentos Invalidos.\n");
    return RES_OK;
}
