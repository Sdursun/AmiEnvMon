#ifndef HTTPGET_H
#define HTTPGET_H

// Gets the http response string from the server.
// (hardcoded for our use case at the moment with little configuration).
// User is responsible for freeing char pointer result.
//
// inputs: 
// - base_url - the base url for the environment monitor web server
//
// Returns NULL on errors.
char* httpget(const char* base_url);

#endif
