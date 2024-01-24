#include "request.h"

http_request_t* parse_request(char* request_str, const char *PATH){
	http_request_t *REQ = (http_request_t *) malloc(sizeof(http_request_t));
	sscanf(request_str, "%s %s %*s", REQ->method, REQ->path);
	if (strcmp(REQ->path, "/favicon.ico") == 0) {
		free(REQ);
		return NULL;
	}
	char *copy_path = malloc(strlen(PATH) + strlen(REQ->path) + 1);
	if (copy_path == NULL) {
		perror("Error: failed to allocate memory for copy_path");
		exit(EXIT_FAILURE);
	}
	strcpy(copy_path, PATH);
	strcat(copy_path, REQ->path);
	strcpy(REQ->path, copy_path);
	printf("method: %s\n", REQ->method);
	printf("path: %s\n", REQ->path);
	free(copy_path);
	return REQ;
}



http_response_t* process_request(http_request_t* request){
	char get_method[] = "GET";
	if(strcmp(get_method, request->method) == 0){
		char* path_copy = strndup(request->path, strlen(request->path));
		char* abs_path = get_abs_path(path_copy);
		char *content = get_content(abs_path);
		return get_response(abs_path, content);

	} else {
		perror("not supported method");
		return NULL;
	}
}

char* get_abs_path(char* request_path)
{
	char *copy_path = malloc(1024);
	strcpy(copy_path, request_path);
	struct stat file_info;
	if (stat(copy_path, &file_info) != 0) {
		perror("Error al obtener información del archivo");
		return NULL;
	}

	// Verificar si es un archivo regular
	if (S_ISREG(file_info.st_mode)) {
		printf("%s es un archivo regular\n", copy_path);
		return copy_path;
	}

	// Verificar si es un directorio
	if (S_ISDIR(file_info.st_mode)) {
		printf("%s es un directorio\n", copy_path);
		// Si no es un archivo concreto, verificamos si el path relativo
		//no tiene extensión.
		// No tiene extensión o tiene una barra después del último punto
		strcat(copy_path, "index.html");
		return copy_path;
	}
	return NULL;
}

char *get_content(const char *PATH){
	FILE *file;
	printf("path abs: \n%s\n", PATH);
	file = fopen(PATH, "rb");
	if (file == NULL)
	{
	printf("Error opening file: %s\n", strerror(errno));
	return NULL;
	}

	fseek(file, 0L, SEEK_END);
	long int size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	char *buffer = (char*) malloc(size + 10);
	fread(buffer, 1, size, file);
	buffer[size] = '\0';
	fclose(file);

	return buffer;
}

http_response_t *get_response(const char *PATH, char *content)
{
	FILE *file = fopen(PATH, "rb");

	fseek(file, 0L, SEEK_END);
	long content_lenght = ftell(file);
	fclose(file);

	char *status_code = "200";
	char *status = "OK";
	const char *content_type = get_content_type(PATH);

	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	char date[64];
	strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", tm);

	const char *http_response_template = "HTTP/1.1 %s %s\r\n"
							  "Content-Type: %s\r\n"
							  "Content-Length: %ld\r\n"
							  "Date: %s\r\n"
							  "Server: Telematics Web Server\r\n"
							  "\r\n%s\r\n";
	size_t buffer_lenght =
		snprintf(NULL, 0, http_response_template, status_code, status,
	content_type, content_lenght, date, content) + 10;
	char *response = (char*) malloc(buffer_lenght+10);

	sprintf(response, http_response_template, status_code, status,
	content_type, content_lenght, date, content);

	http_response_t *real_response =
		(http_response_t*) malloc(sizeof(http_response_t)+10);
	real_response->content = (char*) malloc(buffer_lenght+10);
	memcpy(real_response->content, response, buffer_lenght+10);
	real_response->length = buffer_lenght;

	printf("length : %ld\n", real_response->length);
	printf("content-type : %s\n", content_type);
	free(response);
	return real_response;

}

const char* get_content_type(const char* filename) {
	const char* extension = strrchr(filename, '.');
	if (extension == NULL) {
		return "text/plain"; // tipo de archivo desconocido
	}
	if (strcmp(extension, ".html") == 0 || strcmp(extension, ".htm") == 0) {
		return "text/html";
	} else if (strcmp(extension, ".txt") == 0) {
		return "text/plain";
	} else if (strcmp(extension, ".jpg")  == 0 ||
			   strcmp(extension, ".jpeg") == 0) {
		return "image/jpeg";
	} else if (strcmp(extension, ".gif") == 0) {
		return "image/gif";
	} else if (strcmp(extension, ".png") == 0) {
		return "image/png";
	} else {
		return "text/plain"; // tipo de archivo desconocido
	}
}
