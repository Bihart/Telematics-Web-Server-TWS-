#ifndef REQUEST_H
#define REQUEST_H   

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_PATH_LENGTH 1024

struct http_request
{
    char method[10];
    char path[1024];
};

typedef struct http_response
{
    char *content;
    size_t length;

} http_response_t;


typedef struct http_request http_request_t;

http_request_t *parse_request(char* request_str, const char * PATH);
http_response_t *process_request(http_request_t* request);
char* get_abs_path(char* request);
char *get_content(const char *PATH);
http_response_t *get_response(const char *PATH, char *content);
const char* get_content_type(const char* filename);

#endif