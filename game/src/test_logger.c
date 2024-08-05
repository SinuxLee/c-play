#include "logger.h"

int main(int argc, char **argv)
{
    LoggerOpt opt = {
        .file_name = "test.log",
        .level = "TRACE"
    };

    init_logger(&opt);

    LOG_TRACE("Hello, World!");
    LOG_DEBUG("Hello, World!");
    LOG_INFO("Hello, World!");
    LOG_WARN("Hello, World!");
    LOG_ERROR("Hello, World!");
    LOG_CRIT("Hello, World!");
    LOG_FATAL("Hello, World!");
}