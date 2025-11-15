#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>

#include "platform_thread.h"
#include "ring_queue.h"

// 消息结构体
typedef struct
{
    uint16_t id;
    void *data;
} Message;

typedef void (*Handler)(Message *);

// Worker结构体
typedef struct
{
    platform_thread_t thread;
    atomic_bool running;
    ring_queue *message_queue;
    Handler *handlers;
} Worker;

Worker *create_worker();
void stop_worker(Worker *);
void add_message(Worker *, Message);
bool register_handler(Worker *, uint16_t, Handler);
