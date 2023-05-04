#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024

char* get_absolute_path(char* relative_path) {
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
    if(strchr(relative_path, '/'))
    {
        strcat(absolute_path, relative_path+1);
        return(absolute_path);
    }
    // Concatenamos el path relativo al path absoluto
    strcat(absolute_path, "/");
    strcat(absolute_path, relative_path);

    // Verificamos si el path absoluto apunta a un archivo concreto
    if (access(absolute_path, F_OK) != -1) {
        return absolute_path;
    }

    // Si no es un archivo concreto, verificamos si el path relativo no tiene extensión
    char *extension = strrchr(relative_path, '.');
    if (extension == NULL || strchr(extension, '/') != NULL) {
        // No tiene extensión o tiene una barra después del último punto
        strcat(absolute_path, "/index.html");
    }

    return absolute_path;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <path relativo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *absolute_path = get_absolute_path(argv[1]);

    if (absolute_path == NULL) {
        exit(EXIT_FAILURE);
    }

    printf("Path absoluto: %s\n", absolute_path);

    free(absolute_path);
    exit(EXIT_SUCCESS);
}