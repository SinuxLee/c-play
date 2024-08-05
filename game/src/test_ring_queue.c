#include "ring_queue.h"
#include <stdio.h>

typedef struct
{
    int x;
    float y;
} custom_data_t;

int main()
{
    ring_queue *q = ring_queue_new(sizeof(custom_data_t), 3);
    custom_data_t data1 = {1, 1.1f};
    custom_data_t data2 = {2, 2.2f};
    custom_data_t data3 = {3, 3.3f};

    ring_queue_enqueue(q, &data1);
    ring_queue_enqueue(q, &data2);
    ring_queue_enqueue(q, &data3);

    custom_data_t result;
    ring_queue_dequeue(q, &result);
    // printf("Dequeued: %d, %f\n", result.x, result.y); // 输出1, 1.1

    ring_queue_dequeue(q, &result);
    // printf("Dequeued: %d, %f\n", result.x, result.y); // 输出2, 2.2

    ring_queue_free(q);
    return 0;
}