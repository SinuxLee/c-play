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
    mtx_init(&buf->lock, mtx_plain);
}

/* 销毁缓冲区 */
static void buffer_free(buffer_t *buf)
{
    free(buf->buffer);
    mtx_destroy(&buf->lock);
}

/* 初始化环形队列 */
ring_queue * ring_queue_new(size_t elem_size, size_t max_size)
{
    ring_queue *q = (ring_queue *)malloc(sizeof(ring_queue));
    buffer_init(&q->buffers[0], elem_size, max_size);
    buffer_init(&q->buffers[1], elem_size, max_size);
    q->write_buf = &q->buffers[0];
    q->read_buf = &q->buffers[1];
    mtx_init(&q->swap_lock, mtx_plain);
    return q;
}

/* 销毁环形队列 */
void ring_queue_free(ring_queue *q)
{
    buffer_free(&q->buffers[0]);
    buffer_free(&q->buffers[1]);
    mtx_destroy(&q->swap_lock);
    free(q);
}

/* 入队操作 */
void ring_queue_enqueue(ring_queue *q, void *item)
{
    // 入队不需要交换队列
    buffer_t *buf = q->write_buf;

    mtx_lock(&buf->lock);
    memcpy((char *)buf->buffer + buf->head * buf->elem_size, item, buf->elem_size);
    buf->head = (buf->head + 1) % buf->max;

    if (buf->head == buf->tail)
    {
        buf->full = true;
    }
    mtx_unlock(&buf->lock);

    mtx_lock(&q->swap_lock);
    if (buf->full)
    {
        mtx_lock(&q->read_buf->lock);
        buffer_t *temp = q->write_buf;
        q->write_buf = q->read_buf;
        q->read_buf = temp;
        q->write_buf->head = 0;
        q->write_buf->tail = 0;
        q->write_buf->full = false;
        mtx_unlock(&q->read_buf->lock);
    }
    mtx_unlock(&q->swap_lock);
}

/* 出队操作 */
bool ring_queue_dequeue(ring_queue *q, void *item)
{
    // 出队时，可能需要交互队列
    buffer_t *buf = q->read_buf;
    bool success = false;

    mtx_lock(&buf->lock);
    if (buf->tail != buf->head || buf->full)
    {
        memcpy(item, (char *)buf->buffer + buf->tail * buf->elem_size, buf->elem_size);
        buf->tail = (buf->tail + 1) % buf->max;
        buf->full = false;
        success = true;
    }
    mtx_unlock(&buf->lock);

    mtx_lock(&q->swap_lock);
    if (buf->tail == buf->head && q->write_buf->head != q->write_buf->tail)
    {
        mtx_lock(&q->write_buf->lock);
        buffer_t *temp = q->write_buf;
        q->write_buf = q->read_buf;
        q->read_buf = temp;
        mtx_unlock(&q->write_buf->lock);
    }
    mtx_unlock(&q->swap_lock);

    return success;
}

#ifdef TEST
typedef struct
{
    int x;
    float y;
} custom_data_t;

int main()
{
    ring_queue q;
    ring_queue_init(&q, sizeof(custom_data_t), 5);

    custom_data_t data1 = {1, 1.1f};
    custom_data_t data2 = {2, 2.2f};
    custom_data_t data3 = {3, 3.3f};

    ring_queue_enqueue(&q, &data1);
    ring_queue_enqueue(&q, &data2);
    ring_queue_enqueue(&q, &data3);

    custom_data_t result;
    ring_queue_dequeue(&q, &result);
    printf("Dequeued: %d, %f\n", result.x, result.y); // 输出1, 1.1

    ring_queue_dequeue(&q, &result);
    printf("Dequeued: %d, %f\n", result.x, result.y); // 输出2, 2.2

    ring_queue_free(&q);
    return 0;
}
#endif
