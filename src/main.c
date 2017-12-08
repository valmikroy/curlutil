#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>
#include <curl/curl.h>
#include <curl/easy.h>


#include "main.h"
#include "log.h"
#include "curl.h"


static const char *optString = "u:t:vn?";



static const struct option longOpts[] = {
   { "url", required_argument, NULL, 'u' },
   { "timeout", required_argument, NULL, 't' },
   { "verbose", no_argument,NULL,'v'},
   { "notlsreuse", no_argument,NULL,'n'},
   // we need timeout option
};


void display_usage( void ) {
    puts( "Fetch given URL using easy libcurl functions");
    puts( "-u --url                 \t\t URL to fetch");
    puts( "-t --timeout             \t\t Timeout for fetch loop");
    puts( "-n --notlsreuse          \t\t No TLS reuse");
    puts( "-v --verbose             \t\t Verbose logging");
    exit( EXIT_FAILURE );
}

void timestamp(void) {
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    LOG_INFO("level=INFO ts=%lu.%lu ",(uint64_t)curTime.tv_sec, (uint64_t)curTime.tv_usec);
}

void sleep_rand_ms(int t) {
  usleep( ( rand() % t ) * 1000 );
}

void fetch_loop(void) {
  if ( cmdArgs.url == NULL )
      display_usage();

  LOG_INFO("tls: %d\n", cmdArgs.tlsreuse);
  LOG_INFO("url: %s\n", cmdArgs.url);
  LOG_INFO("verbose: %d\n", cmdArgs.verbose);
  LOG_INFO("timeout: %d\n", cmdArgs.timeout);


  CURL *c = curlInitialize(NULL,NULL,NULL);

  int start = (int)time(NULL);

  for(;;) {
    int now = (int)time(NULL);
    if ( (now - start) > cmdArgs.timeout ) {
      LOG_INFO("Loop timeout %d seconds has reached\n", cmdArgs.timeout);
      break;
    }
    sleep_rand_ms(RANDOM_WAIT);
    c = ( cmdArgs.tlsreuse < 1 ) ? curlInitialize(NULL,cmdArgs.url,NULL) : \
                                curlInitialize(c,cmdArgs.url,NULL);
    if((curlFetch(c)) && (isHTTP200(c))) {
      timestamp();
      curlTimeinfo(c);
    }
    else {
      LOG_ERROR("Non-200 HTTP response, aborting loop\n");
      exit( EXIT_FAILURE );
    }
  }

}




int main( int argc, char *argv[] ) {
    int opt = 0;
    int longIndex = 0;

    cmdArgs.url = NULL;
    cmdArgs.verbose = 0;
    cmdArgs.tlsreuse = 1;
    cmdArgs.timeout = TIMEOUT;


    opt = getopt_long(argc, argv, optString, longOpts, &longIndex );
    while (opt != -1 ) {
        switch( opt ){
            case 'u':
                cmdArgs.url = optarg;
                break;
            case 't':
                cmdArgs.timeout = atoi(optarg);
                break;
            case 'v':
                cmdArgs.verbose++;
                cmdArgs.verbose++;
                break;
            case 'n':
                cmdArgs.tlsreuse--;
                break;
            case 'h':
            case '?':
                display_usage();
                break;
            default:
                /*unreachable*/
                break;
        }

        opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    }

    fetch_loop();

    return EXIT_SUCCESS;

}
