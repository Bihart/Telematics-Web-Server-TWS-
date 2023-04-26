#include "request.h"

http_request_t* parse_request(char* request_str){
    http_request_t *REQ = (http_request_t *) malloc(sizeof(http_request_t));
    sscanf(request_str, "%s %s %*s", REQ->method, REQ->path);

    return REQ;
}

char* process_request(http_request_t* request){
    char get_method[] = "GET";
    if(strcmp(get_method, request->method) == 0){
        get_abs_path(request->path); 
    } else {
        perror("not suported method");
    }
}

char* get_abs_path(http_request_t* request)
{
    char *absolute_path = malloc(MAX_PATH_LENGTH);

    if (absolute_path == NULL) {
        perror("Error al asignar memoria");
        return NULL;
    }

    // Obtenemos el directorio actual
    if (getcwd(absolute_path, MAX_PATH_LENGTH) == NULL) {
        perror("Error al obtener el directorio actual");
        free(absolute_path);
        return NULL;
    }

    // Concatenamos el path relativo al path absoluto
    strcat(absolute_path, "/");
    strcat(absolute_path, request);

    // Verificamos si el path absoluto apunta a un archivo concreto
    if (access(absolute_path, F_OK) != -1) {
        return absolute_path;
    }

    // Si no es un archivo concreto, verificamos si el path relativo no tiene extensión
    char *extension = strrchr(request, '.');
    if (extension == NULL || strchr(extension, '/') != NULL) {
        // No tiene extensión o tiene una barra después del último punto
        strcat(absolute_path, "/index.html");
    }

    return absolute_path;
}

