#include "worker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 一个示例Handler
void example_handler(Message *msg) {
    printf("Handling message with ID: %d, data: %s\n", msg->id, (char *)msg->data);
}

// 使用示例
int main() {
    Worker *worker = create_worker();
    register_handler(worker, 1, example_handler);

    char *data = calloc(16, sizeof(char));
    memcpy(data, "Hello, World!", strlen("Hello, World!"));

    Message msg1 = { 1, data };
    add_message(worker, msg1);

    thrd_sleep(&(struct timespec){ .tv_sec = 1 }, NULL); // 给线程一些时间来处理消息
    stop_worker(worker);

    return 0;
}