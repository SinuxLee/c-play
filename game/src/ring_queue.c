#include "ring_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 初始化缓冲区 */
static void buffer_init(buffer_t *buf, size_t elem_size, size_t max_size)
{
    buf->buffer = malloc(max_size * elem_size);
    buf->head = 0;
    buf->tail = 0;
    buf->max = max_size;
    buf->elem_size = elem_size;
    buf->full = false;
}

/* 销毁缓冲区 */
static void buffer_free(buffer_t *buf)
{
    free(buf->buffer);
}

static void buffer_push(buffer_t *buf, void *item)
{
    memcpy((char *)buf->buffer + buf->head * buf->elem_size, item, buf->elem_size);
    buf->head = (buf->head + 1) % buf->max;

    if (buf->head == buf->tail)
    {
        buf->full = true;
    }
}

static bool buffer_pop(buffer_t *buf, void *item)
{
    bool success = false;
    if (buf->tail != buf->head || buf->full)
    {
        memcpy(item, (char *)buf->buffer + buf->tail * buf->elem_size, buf->elem_size);
        buf->tail = (buf->tail + 1) % buf->max;
        buf->full = false;
        success = true;
    }

    return success;
}

/* 初始化环形队列 */
ring_queue * ring_queue_new(size_t elem_size, size_t max_size)
{
    ring_queue *q = (ring_queue *)malloc(sizeof(ring_queue));
    buffer_init(&q->buffers[0], elem_size, max_size);
    buffer_init(&q->buffers[1], elem_size, max_size);
    q->write_buf = &q->buffers[0];
    q->read_buf = &q->buffers[1];
    PLATFORM_MUTEX_INIT(&q->swap_lock);
    return q;
}

/* 销毁环形队列 */
void ring_queue_free(ring_queue *q)
{
    buffer_free(&q->buffers[0]);
    buffer_free(&q->buffers[1]);
    PLATFORM_MUTEX_DESTROY(&q->swap_lock);
    free(q);
}

/* 入队操作 */
void ring_queue_enqueue(ring_queue *q, void *item)
{
    PLATFORM_MUTEX_LOCK(&q->swap_lock);

    buffer_t *buf = q->write_buf;
    buffer_push(buf, item);

    if (buf->full)
    {
        buffer_t *temp = q->write_buf;
        q->write_buf = q->read_buf;
        q->read_buf = temp;
    }

    PLATFORM_MUTEX_UNLOCK(&q->swap_lock);
}

/* 出队操作 */
bool ring_queue_dequeue(ring_queue *q, void *item)
{
    PLATFORM_MUTEX_LOCK(&q->swap_lock);
    buffer_t *buf = q->read_buf;

    bool success = buffer_pop(buf,item);
    if (!success)
    {
        buf = q->write_buf;
        q->write_buf = q->read_buf;
        q->read_buf = buf;
        success = buffer_pop(buf,item);
    }

    PLATFORM_MUTEX_UNLOCK(&q->swap_lock);
    return success;
}
