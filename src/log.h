#ifndef LOG_H
#define LOG_H

typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR
} LOG_LEVEL;

extern const char * log_type[4];
extern const char * colors[4];

void log_message(LOG_LEVEL ll, char *file, int line, const char *function, char *log_message_fmt, ...);

#define LOG_DEBUG(fmt, ...) log_message(DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) log_message(INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) log_message(WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) log_message(ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif // !LOG_H
