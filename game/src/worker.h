#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>
#include <threads.h>

// 消息结构体
typedef struct {
    int id;
    void *data;
} Message;

typedef void(*Handler)(Message *);

// Worker结构体
typedef struct {
    thrd_t thread;
    atomic_bool running;
    Message *message_queue;
    size_t queue_size;
    size_t queue_capacity;
    mtx_t queue_mutex;
    cnd_t queue_cond;
    Handler *handlers;
} Worker;

void init_worker(Worker *worker);
void stop_worker(Worker *worker);
void add_message(Worker *worker, Message msg);
bool register_handler(Worker *worker, uint16_t id, Handler handler);
