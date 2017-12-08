#define L_INFO 0x01
#define L_WARN 0x02
#define L_ERROR 0x03
#define L_VERBOSE 0x04
#define L_DEBUG 0x05

#define LOG_INFO(...) log_print(L_INFO, __VA_ARGS__)
#define LOG_WARN(...) log_print(L_WARN, __VA_ARGS__)
#define LOG_ERROR(...) log_print(L_ERROR, __VA_ARGS__)
#define LOG_VERBOSE(...) log_print(L_VERBOSE, __VA_ARGS__)
#define LOG_DEBUG(...) log_print(L_DEBUG, __VA_ARGS__)

int log_print(int lvl, char *fmt, ...);
