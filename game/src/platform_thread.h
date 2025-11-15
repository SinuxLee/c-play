#pragma once
#include <stdbool.h>
#include <stdint.h>

// 跨平台线程类型和函数
#if defined(__APPLE__) || defined(__MACH__)
    // macOS 使用 pthread
    #include <pthread.h>
    typedef pthread_t platform_thread_t;
    typedef void* (*platform_thread_func_t)(void*);
    
    static inline int platform_thread_create(platform_thread_t *thr, platform_thread_func_t func, void *arg) {
        return pthread_create(thr, NULL, func, arg);
    }
    
    static inline int platform_thread_join(platform_thread_t thr) {
        void *retval;
        return pthread_join(thr, &retval);
    }
    
    // 线程函数返回类型转换
    #define PLATFORM_THREAD_FUNC(func) ((platform_thread_func_t)(func))
    
#elif defined(_WIN32) || defined(_WIN64)
    // Windows 使用 Windows API
    #include <windows.h>
    typedef HANDLE platform_thread_t;
    typedef DWORD (WINAPI *platform_thread_func_t)(void*);
    
    static inline int platform_thread_create(platform_thread_t *thr, platform_thread_func_t func, void *arg) {
        *thr = CreateThread(NULL, 0, func, arg, 0, NULL);
        return (*thr != NULL) ? 0 : -1;
    }
    
    static inline int platform_thread_join(platform_thread_t thr) {
        WaitForSingleObject(thr, INFINITE);
        CloseHandle(thr);
        return 0;
    }
    
    #define PLATFORM_THREAD_FUNC(func) ((platform_thread_func_t)(func))
    
#else
    // Linux 和其他平台使用 C11 threads.h
    #include <threads.h>
    typedef thrd_t platform_thread_t;
    typedef int (*platform_thread_func_t)(void*);
    
    static inline int platform_thread_create(platform_thread_t *thr, platform_thread_func_t func, void *arg) {
        return thrd_create(thr, func, arg);
    }
    
    static inline int platform_thread_join(platform_thread_t thr) {
        int res;
        return thrd_join(thr, &res);
    }
    
    #define PLATFORM_THREAD_FUNC(func) (func)
#endif

