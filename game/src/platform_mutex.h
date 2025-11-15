#pragma once
#include <stdbool.h>

// 跨平台互斥锁类型
#if defined(__APPLE__) || defined(__MACH__)
    // macOS 使用 pthread
    #include <pthread.h>
    typedef pthread_mutex_t platform_mutex_t;
    #define PLATFORM_MUTEX_INIT(mtx) pthread_mutex_init(mtx, NULL)
    #define PLATFORM_MUTEX_LOCK(mtx) pthread_mutex_lock(mtx)
    #define PLATFORM_MUTEX_UNLOCK(mtx) pthread_mutex_unlock(mtx)
    #define PLATFORM_MUTEX_DESTROY(mtx) pthread_mutex_destroy(mtx)
#elif defined(_WIN32) || defined(_WIN64)
    // Windows 使用 Windows API
    #include <windows.h>
    typedef CRITICAL_SECTION platform_mutex_t;
    #define PLATFORM_MUTEX_INIT(mtx) InitializeCriticalSection(mtx)
    #define PLATFORM_MUTEX_LOCK(mtx) EnterCriticalSection(mtx)
    #define PLATFORM_MUTEX_UNLOCK(mtx) LeaveCriticalSection(mtx)
    #define PLATFORM_MUTEX_DESTROY(mtx) DeleteCriticalSection(mtx)
#else
    // Linux 和其他平台使用 C11 threads.h
    #include <threads.h>
    typedef mtx_t platform_mutex_t;
    #define PLATFORM_MUTEX_INIT(mtx) mtx_init(mtx, mtx_plain)
    #define PLATFORM_MUTEX_LOCK(mtx) mtx_lock(mtx)
    #define PLATFORM_MUTEX_UNLOCK(mtx) mtx_unlock(mtx)
    #define PLATFORM_MUTEX_DESTROY(mtx) mtx_destroy(mtx)
#endif

