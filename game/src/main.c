#include <stdio.h>
#include "logger.h"
#include "player_mgr.h"
#include "room_mgr.h"
#include "global.h"
#include "ring_queue.h"

int main(int argc, char **argv)
{
    LoggerOpt opt = {
        .file_name = "log.txt",
        .level = "debug"};
    init_logger(&opt);
    LOG_INFO("bin building on %s %s",__DATE__,__TIME__);

    Global *global = get_global();
    Player *p = create_player(10086);
    p = create_player(10010);
    p = create_player(10000);

    p = find_player(10010);
    player_to_string(p);
    print_players();

    ring_queue *q = ring_queue_new(sizeof(Player), 10000);
    for (size_t i = 0; i < 10; i++)
    {
        Player p = {.id = i};
        sprintf(p.name, "name %02ld", i);
        ring_queue_enqueue(q, &p);
    }

    Player pp;
    while (ring_queue_dequeue(q, &pp))
    {
        player_to_string(&pp);
    }

    ring_queue_free(q);

    sleep(2);

    return EXIT_SUCCESS;
}
