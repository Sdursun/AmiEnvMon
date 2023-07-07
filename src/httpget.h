#ifndef HTTPGET_H
#define HTTPGET_H

// Gets the http response string from the server.
// (hardcoded for our use case at the moment with little configuration).
// User is responsible for freeing char pointer result.
//
// Returns NULL on errors.
char* httpget();

#endif
