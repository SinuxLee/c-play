#include "worker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint16_t HASH_TABLE_SIZE =100;

// 哈希函数
static inline size_t hash(int id) {
    return (size_t) (id % HASH_TABLE_SIZE);
}

// Worker处理消息的线程函数
static int worker_thread_func(void *arg) {
    Worker *worker = (Worker *)arg;

    while (atomic_load(&worker->running)) {
        mtx_lock(&worker->queue_mutex);
        
        while (worker->queue_size == 0) {
            cnd_wait(&worker->queue_cond, &worker->queue_mutex);
        }

        Message msg = worker->message_queue[--worker->queue_size];
        
        mtx_unlock(&worker->queue_mutex);

        size_t idx = hash(msg.id);
        Handler handler = worker->handlers[idx];

        if (handler == NULL) {
            printf("No handler found for message ID: %d\n", msg.id);
            continue;
        }
        
        handler(&msg);
    }

    return EXIT_SUCCESS;
}

// 初始化Worker结构体
void init_worker(Worker *worker) {
    worker->running = true;
    worker->queue_size = 0;
    worker->queue_capacity = 10000;
    worker->message_queue = malloc(worker->queue_capacity * sizeof(Message));
    mtx_init(&worker->queue_mutex, mtx_plain);
    cnd_init(&worker->queue_cond);
    worker->handlers = calloc(HASH_TABLE_SIZE, sizeof(Handler));
    thrd_create(&worker->thread, worker_thread_func, worker);
}

// 停止Worker
void stop_worker(Worker *worker) {
    atomic_store(&worker->running, false);
    cnd_signal(&worker->queue_cond);
    thrd_join(worker->thread, NULL);
    mtx_destroy(&worker->queue_mutex);
    cnd_destroy(&worker->queue_cond);
    free(worker->handlers);
    free(worker->message_queue);
}

// 向Worker添加消息
void add_message(Worker *worker, Message msg) {
    mtx_lock(&worker->queue_mutex);

    if (worker->queue_size == worker->queue_capacity) {
        worker->queue_capacity *= 2;
        worker->message_queue = realloc(worker->message_queue, worker->queue_capacity * sizeof(Message));
    }

    worker->message_queue[worker->queue_size++] = msg;
    cnd_signal(&worker->queue_cond);

    mtx_unlock(&worker->queue_mutex);
}

// 向HashTable添加Handler
bool register_handler(Worker *worker, uint16_t id, Handler handler) {
    size_t idx = hash(id);
    worker->handlers[idx] = handler;
}

