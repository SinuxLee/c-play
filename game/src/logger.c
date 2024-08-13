#include "logger.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <threads.h>

#include "utringbuffer.h"

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define delay(ms) Sleep(ms)
#else
#include <unistd.h>
#define delay(ms) usleep((ms)*1000)
#endif

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

static const uint16_t LOG_SIZE = 1023;

typedef struct {
  char *data;
} LogData;

typedef struct {
  UT_ringbuffer *logs;
  mtx_t mutex; // protect logs

  LOG_LEVEL level;
  const char *file_name;
  thrd_t pth;
  bool quit;
} Logger;

static Logger *logger = NULL;
static const char *levels[] = {"off",  "fatal", "crit",  "error", "warn",
                               "info", "debug", "trace", NULL};

static void print(const char *file, uint32_t line, const char * func, const char *level, const char *format, va_list args) {
  char *buffer = (char *)calloc(1, LOG_SIZE + 1);

  struct timespec ts;
  timespec_get(&ts, TIME_UTC);

  size_t len = strftime(buffer, LOG_SIZE, "%F %T", gmtime(&ts.tv_sec));
  len += snprintf(buffer + len, LOG_SIZE - len, ".%03ld %s %s:%d [%s] ", ts.tv_nsec / 1000000, level, file,line,func);

  vsnprintf(buffer + len, LOG_SIZE - len, format, args);
  LogData data = {.data = buffer};
  mtx_lock(&logger->mutex);
  utringbuffer_push_back(logger->logs, &data);
  mtx_unlock(&logger->mutex);
  free(buffer);
}

static int handle_log(void *data){
  Logger *logger = (Logger*)data;
  LogData *item = NULL;
  while(!logger->quit){
    mtx_lock(&logger->mutex);
    LogData *next = (LogData*)utringbuffer_next(logger->logs, item);
    mtx_unlock(&logger->mutex);
    if(next == NULL){
      delay(10);
      continue;
    }

    item = next;
    printf("%s\n", item->data);
  }
  utringbuffer_free(logger->logs);
  return EXIT_SUCCESS;
}

static void copy_log_data(void *_dst, const void *_src) {
  LogData *dst = (LogData*)_dst, *src = (LogData*)_src;
  dst->data = src->data ? strdup(src->data) : NULL;
}

static void destory_log_data(void *_elt) {
  LogData *elt = (LogData*)_elt;
  free(elt->data);
}

static UT_icd log_data_icd = {sizeof(LogData), NULL, copy_log_data, destory_log_data};

bool init_logger(LoggerOpt *opt) {
  if(opt == NULL) return false;

  LOG_LEVEL lv = INFO;
  for (int idx = 1; levels[idx] != NULL; idx++) {
    if (strcasecmp(opt->level, levels[idx]) == 0) {
      lv = idx;
      break;
    }
  }

  static Logger l = {.mutex = mtx_plain, .quit=false};
  logger = &l;
  logger->level = lv;
  logger->file_name=opt->file_name;
  utringbuffer_new(logger->logs, 50000, &log_data_icd);
  thrd_create(&logger->pth, handle_log, (void *)logger);

  if (opt->file_name == NULL)
    logger->file_name = "log.txt";

  return true;
}

void log_trace(const char *file, uint32_t line, const char * func,const char *format, ...) {
  if (logger->level < TRACE)
    return;

  va_list args;
  va_start(args, format);
  print(file,line,func,"TRACE", format, args);
  va_end(args);
}

void log_debug(const char *file, uint32_t line, const char * func,const char *format, ...) {
  if (logger->level < DEBUG)
    return;

  va_list args;
  va_start(args, format);
  print(file,line,func,"DEBUG", format, args);
  va_end(args);
}

void log_info(const char *file, uint32_t line, const char * func, const char *format, ...) {
  if (logger->level < INFO)
    return;

  va_list args;
  va_start(args, format);
  print(file,line,func,"INFO", format, args);
  va_end(args);
}

void log_warn(const char *file, uint32_t line, const char * func,const char *format, ...) {
  if (logger->level < WARNING)
    return;

  va_list args;
  va_start(args, format);
  print(file,line,func,"WARN", format, args);
  va_end(args);
}

void log_error(const char *file, uint32_t line, const char * func,const char *format, ...) {
  if (logger->level < ERROR)
    return;

  va_list args;
  va_start(args, format);
  print(file,line,func,"ERROR", format, args);
  va_end(args);
}

void log_crit(const char *file, uint32_t line, const char * func,const char *format, ...) {
  if (logger->level < CRITICAL)
    return;

  va_list args;
  va_start(args, format);
  print(file,line,func,"CRIT", format, args);
  va_end(args);
}

void log_fatal(const char *file, uint32_t line, const char * func,const char *format, ...) {
  if (logger->level < FATAL)
    return;

  va_list args;
  va_start(args, format);
  print(file,line,func,"FATAL", format, args);
  va_end(args);
}
