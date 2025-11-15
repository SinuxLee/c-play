#include "worker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "logger.h"

static void delay(uint16_t ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

// Worker处理消息的线程函数
#if defined(__APPLE__) || defined(__MACH__)
static void* worker_thread_func(void *arg)
#elif defined(_WIN32) || defined(_WIN64)
static DWORD WINAPI worker_thread_func(void *arg)
#else
static int worker_thread_func(void *arg)
#endif
{
    Worker *worker = (Worker *)arg;
    Message msg;
    while (atomic_load(&worker->running))
    {
        bool ok = ring_queue_dequeue(worker->message_queue, &msg);
        if (!ok)
        {
            delay(10);
            continue;
        }

        Handler handler = worker->handlers[msg.id];
        if (handler == NULL)
        {
            LOG_ERROR("No handler found for message ID: %d", msg.id);
            free(msg.data);
            continue;
        }

        handler(&msg);
        free(msg.data);
    }

#if defined(__APPLE__) || defined(__MACH__)
    return NULL;
#elif defined(_WIN32) || defined(_WIN64)
    return 0;
#else
    return EXIT_SUCCESS;
#endif
}

// 初始化Worker结构体
Worker *create_worker()
{
    Worker *worker = malloc(sizeof(Worker));
    worker->running = true;
    worker->message_queue = ring_queue_new(sizeof(Message), INT16_MAX);
    worker->handlers = calloc(INT16_MAX, sizeof(Handler));
    platform_thread_create(&worker->thread, PLATFORM_THREAD_FUNC(worker_thread_func), worker);
    return worker;
}

// 停止Worker
void stop_worker(Worker *worker)
{
    atomic_store(&worker->running, false);
    platform_thread_join(worker->thread);
    ring_queue_free(worker->message_queue);
    free(worker->handlers);
    free(worker);
}

// 向Worker添加消息
void add_message(Worker *worker, Message msg)
{
    ring_queue_enqueue(worker->message_queue, &msg);
}

// 向HashTable添加Handler
bool register_handler(Worker *worker, uint16_t id, Handler handler)
{
    worker->handlers[id] = handler;
    return true;
}
