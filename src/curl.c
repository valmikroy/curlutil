#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>
#include <string.h>

#include "curl.h"
#include "log.h"



CURL * curlInitialize(CURL *c, char *url,  struct curlWriteStruct *chunk) {
  FILE *devnull = fopen("/dev/null", "w+");

  if (c == NULL)
    c = curl_easy_init();

  curl_easy_setopt(c, CURLOPT_USERAGENT, CURL_USERAGENT);
  curl_easy_setopt(c, CURLOPT_TIMEOUT, CURL_TIMEOUT);

  if ( url != NULL )
    curl_easy_setopt(c, CURLOPT_URL, url);

  if ( chunk == NULL )  {
    curl_easy_setopt(c,CURLOPT_WRITEDATA,devnull);
  }
  else {
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, (void *)chunk);
  }

  return c;
}

int curlFetch(CURL *c) {
  CURLcode res;
  res = curl_easy_perform(c);

  if (res != CURLE_OK) {
     LOG_ERROR("Failed %s\n",curl_easy_strerror(res));
     return 0;
  }
  curlTimeinfo(c);
  return 1;
}

int isHTTP200(CURL *c) {
    CURLcode res;
    long response_code;
    res = curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &response_code);
    return ((CURLE_OK == res) && (response_code>=200) && (response_code<=299)) ? 1 : 0;
}


//curlWriteStruct
size_t curlWriteCallback( void *contents, size_t size, size_t nmemb, void *userp ) {

    size_t realsize = size * nmemb;
    struct curlWriteStruct *mem = (struct curlWriteStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if ( mem->memory == NULL ){
        LOG_ERROR("mem allocation failed\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


void curlTimeinfo(CURL *c) {

    CURLcode res;

    // CURLINFO_RESPONSE_CODE
    long response_code;
    res = curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &response_code);
    if((CURLE_OK == res) && (response_code>0))
    LOG_INFO("response_code=%ld ", response_code );

    //XXX: retry for non-200 response

    // CURLINFO_TOTAL_TIME
    double curl_total_time;
    res = curl_easy_getinfo(c, CURLINFO_TOTAL_TIME, &curl_total_time);
    if((CURLE_OK == res) && (curl_total_time>0))
    LOG_INFO("curl_total_time=%0.3f ", curl_total_time * 1000);

    // CURLINFO_NAMELOOKUP_TIME
    double curl_namelookup_time;
    res = curl_easy_getinfo(c, CURLINFO_NAMELOOKUP_TIME, &curl_namelookup_time);
    if((CURLE_OK == res) && (curl_namelookup_time>0))
    LOG_INFO("curl_namelookup_time=%0.3f ", curl_namelookup_time * 1000);

    // CURLINFO_CONNECT_TIME
    double curl_connect_time;
    res = curl_easy_getinfo(c, CURLINFO_CONNECT_TIME , &curl_connect_time);
    if((CURLE_OK == res) && (curl_connect_time>0))
    LOG_INFO("curl_connect_time=%0.3f ", curl_connect_time * 1000);

    // CURLINFO_APPCONNECT_TIME
    double curl_appconnect_time;
    res = curl_easy_getinfo(c, CURLINFO_APPCONNECT_TIME , &curl_appconnect_time);
    if((CURLE_OK == res) && (curl_appconnect_time>0))
    LOG_INFO("curl_appconnect_time=%0.3f ", curl_appconnect_time * 1000);

    // CURLINFO_PRETRANSFER_TIME
    double curl_pretransfer_time;
    res = curl_easy_getinfo(c, CURLINFO_PRETRANSFER_TIME, &curl_pretransfer_time);
    if((CURLE_OK == res) && (curl_pretransfer_time>0))
    LOG_INFO("curl_pretransfer_time=%0.3f ", curl_pretransfer_time * 1000);

    // CURLINFO_STARTTRANSFER_TIME
    double curl_starttransfer_time;
    res = curl_easy_getinfo(c, CURLINFO_STARTTRANSFER_TIME, &curl_starttransfer_time);
    if((CURLE_OK == res) && (curl_starttransfer_time>0))
    LOG_INFO("curl_starttransfer_time=%0.3f ", curl_starttransfer_time * 1000);

    // CURLINFO_SIZE_DOWNLOAD
    double curl_size_download;
    res = curl_easy_getinfo(c, CURLINFO_SIZE_DOWNLOAD, &curl_size_download);
    if((CURLE_OK == res) && (curl_size_download>0))
    LOG_INFO("curl_size_download=%0.0f ", curl_size_download);

    // CURLINFO_EFFECTIVE_URL
    char *curl_effective_url = NULL;
    curl_easy_getinfo(c, CURLINFO_EFFECTIVE_URL, &curl_effective_url);
    if((CURLE_OK == res) && (curl_effective_url))
        LOG_INFO("curl_effective_url=%s ", curl_effective_url);

    LOG_INFO("\n");

}
