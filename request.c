#include "request.h"

http_request_t* parse_request(char* request_str, const char *PATH){
    http_request_t *REQ = (http_request_t *) malloc(sizeof(http_request_t));
    sscanf(request_str, "%s %*s", REQ->method);
    strcpy(REQ->path, PATH);
    printf("method: %s\n", REQ->method);
    printf("path: %s\n", REQ->path);
    return REQ;
}


char* process_request(http_request_t* request){
    char get_method[] = "GET";
    printf("path1: %s\n", request->path);
    if(strcmp(get_method, request->method) == 0){
        printf("path2: %s\n", request->path);
        char* path_copy = strndup(request->path, strlen(request->path));
        printf("path4: %s\n", request->path);
        char* abs_path = get_abs_path(path_copy);
        free(path_copy);
        return abs_path;
    } else {
        perror("not supported method");
        return NULL;
    }
}

char* get_abs_path(char* request)
{
    char *current_dir = malloc(MAX_PATH_LENGTH);
    if (current_dir == NULL) {
        perror("Error al asignar memoria");
        return NULL;
    }

    // Obtenemos el directorio actual
    if (getcwd(current_dir, MAX_PATH_LENGTH) == NULL) {
        perror("Error al obtener el directorio actual");
        free(current_dir);
        return NULL;
    }

    size_t request_len = strlen(request);
    size_t current_dir_len = strlen(current_dir);
    size_t abs_path_len = request_len + current_dir_len + 2; // Sumamos 2 por el separador y el terminador

    char *absolute_path = malloc(abs_path_len);
    if (absolute_path == NULL) {
        perror("Error al asignar memoria");
        free(current_dir);
        return NULL;
    }

    // Copiamos el directorio actual al path absoluto
    strcpy(absolute_path, current_dir);
    free(current_dir);

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
