#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum LOG_LEVEL_S{
  NONE = 0,     // 关闭日志
  FATAL = 1,    // 无法恢复的致命错误
  CRITICAL = 2, // 记录可能会崩溃的错误
  ERROR = 3,    // 无法继续正常运行的错误
  WARNING = 4,  // 不会导致应用失败警告信息
  INFO = 5,     // 记录重要信息, 生产环境的默认等级
  DEBUG = 6,    // 记录调试信息, 仅开发和测试阶段
  TRACE = 7,    // 详细的调试信息, 仅开发阶段
} LOG_LEVEL;

typedef struct {
  const char *level;
  const char *file_name;
} LoggerOpt;

bool init_logger(LoggerOpt *opt);
void log_trace(const char *file, uint32_t line, const char * func,const char *format, ...);
void log_debug(const char *file, uint32_t line, const char * func,const char *format, ...);
void log_info(const char *file, uint32_t line, const char * func, const char *format, ...);
void log_warn(const char *file, uint32_t line, const char * func,const char *format, ...);
void log_error(const char *file, uint32_t line, const char * func,const char *format, ...);
void log_crit(const char *file, uint32_t line, const char * func,const char *format, ...);
void log_fatal(const char *file, uint32_t line, const char * func,const char *format, ...);

#define LOG_TRACE(fmt,...) log_trace(__FILE__,__LINE__,__func__,fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt,...) log_debug(__FILE__,__LINE__,__func__,fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt,...) log_info(__FILE__,__LINE__,__func__,fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt,...) log_warn(__FILE__,__LINE__,__func__,fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt,...) log_error(__FILE__,__LINE__,__func__,fmt, ##__VA_ARGS__)
#define LOG_CRIT(fmt,...) log_crit(__FILE__,__LINE__,__func__,fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt,...) log_fatal(__FILE__,__LINE__,__func__,fmt, ##__VA_ARGS__)
