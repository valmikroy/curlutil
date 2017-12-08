#define RANDOM_WAIT 300
#define TIMEOUT 15


struct cmdArgs_t {
    int verbose;        /*  -v --verbose    */
    int tlsreuse;       /*  -n --notlsreuse */
    int timeout;   /* -t --timeout */
    char *url;          /*  -u --url        */

    //TODO
    //char *cipher;
} cmdArgs;


void sleep_rand_ms(int t);
void fetch_loop(void);
void timestamp(void);
void display_usage(void);
