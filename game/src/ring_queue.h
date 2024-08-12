#pragma once
#include <stddef.h>
#include <threads.h>
#include <stdbool.h>

typedef struct
{
    void *buffer;
    size_t head;      // 写指针
    size_t tail;      // 读指针
    size_t max;       // 缓冲区大小
    size_t elem_size; // 元素大小
    bool full;        // 缓冲区是否已满
} buffer_t;

typedef struct
{
    buffer_t buffers[2]; // 双缓冲区
    buffer_t *write_buf; // 当前写缓冲区
    buffer_t *read_buf;  // 当前读缓冲区
    mtx_t swap_lock;     // 交换缓冲区的锁
} ring_queue;

ring_queue * ring_queue_new(size_t elem_size, size_t max_size);
void ring_queue_free(ring_queue *q);
void ring_queue_enqueue(ring_queue *q, void *item);
bool ring_queue_dequeue(ring_queue *q, void *item);
