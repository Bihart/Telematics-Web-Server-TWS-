#ifndef REQUEST_H
#define REQUEST_H   

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 1024

struct http_request
{
    char method[10];
    char path[1024];
};

typedef struct http_request http_request_t;

http_request_t* parse_request(char* request_str, const char * PATH);
char* process_request(http_request_t* request);
char* get_abs_path(char* request);

#endif