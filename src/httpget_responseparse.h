#ifndef HTTPGET_RESPONSEPARSE_H
#define HTTPGET_RESPONSEPARSE_H

// TODO: quick doc about the user being responsible for freeing memory
char* getJsonBodyFromHttpResponse(const char* http_response);

#endif
