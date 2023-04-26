#ifndef REQUEST_H
#define REQUEST_H   

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 1024

struct http_request_t
{
    char* method;
    char* path;
};

typedef struct http_request_t http_request_t;

http_request_t* parse_request(char* request_str);
char* process_request(http_request_t* request);
char* get_abs_path(http_request_t* request);

#endif