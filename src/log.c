#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include "log.h"
#include "main.h"

int log_print(int level, char *fmt, ...)
{
    int result = 0;
    int loglevel = cmdArgs.verbose;

    va_list args;


    // Trick to append timestamp automatically
    /*
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    const char fmt_template[] = "ts=%lu.%lu %s";
    int len = snprintf(NULL, 0, fmt_template,(uint64_t)curTime.tv_sec, (uint64_t)curTime.tv_usec, fmt);
    char *new_fmt = malloc(len + 1);
    snprintf(new_fmt, len + 1, fmt_template, (uint64_t)curTime.tv_sec, (uint64_t)curTime.tv_usec, fmt);
    */

    va_start(args, fmt);

    switch ( level ) {
      case L_INFO:
          // enable if automatic TS insertion desired as stated above
          // similar changes to other vfprintf statements below
          //result = vfprintf(stdout, new_fmt, args);
          result = vfprintf(stdout, fmt, args);
          break;
      case L_WARN:
        fputs("level=WARN ", stderr);
        result = vfprintf(stdout, fmt, args);
          break;
      case L_ERROR:
          fputs("level=ERROR ", stderr);
          result = vfprintf(stderr, fmt, args);
          break;
      case L_VERBOSE:
          if (loglevel > 0) {
              fputs("level=VERBOSE", stderr);
              result = vfprintf(stdout, fmt, args);
          }
          break;
      case L_DEBUG:
          if (loglevel > 1) {
              fputs("level=DEBUG ", stdout);
              result = vfprintf(stdout, fmt, args);
          }
          break;
      default:
          /*unreachable*/
          break;
    }

    va_end(args);
    //free(new_fmt);
    fflush(stdout);
    return result;
}
