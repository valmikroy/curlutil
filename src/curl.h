#ifndef CURL_USERAGENT
#define CURL_USERAGENT "curlutil/0.1 (libcurl)"
#endif

#define CURL_TIMEOUT 60


struct curlWriteStruct {
   char *memory;
   size_t size;
};


size_t curlWriteCallback( void *contents, size_t size, size_t nmemb, void *userp );
void curlTimeinfo(CURL *c);
CURL * curlInitialize(CURL *c, char *url,  struct curlWriteStruct *chunk);
int curlFetch(CURL *c);
int isHTTP200(CURL *c); 
