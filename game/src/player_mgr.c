#include "player_mgr.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "logger.h"

typedef struct
{
    Player *players;
} PlayerMgr;

static PlayerMgr *player_mgr()
{
    static PlayerMgr mgr = {.players = NULL};
    return &mgr;
}

Player *create_player(uint64_t pid)
{
    Player *p = (Player *)calloc(1, sizeof(Player));
    if (p == NULL)
        return p;

    p->id = pid;
    HASH_ADD_INT(player_mgr()->players, id, p);
    return p;
}

/**
 * 根据玩家ID查找玩家对象
 *
 * 本函数通过玩家ID在全局玩家管理器中查找对应的玩家对象。如果玩家ID存在，
 * 则返回对应的玩家指针；如果不存在，则返回NULL。
 *
 * @param pid 玩家的唯一标识符，类型为64位无符号整数
 * @return 返回指向玩家对象的指针，如果找不到则返回NULL
 */
Player *find_player(uint64_t pid)
{
    Player *p = NULL;
    HASH_FIND_INT(player_mgr()->players, &pid, p);

    return p;
}

bool remove_player(Player *p)
{
    if (find_player(p->id) == NULL)
        return false;

    HASH_DEL(player_mgr()->players, p);
    free(p);

    return true;
}

bool update_player(uint64_t pid, const char *new_name)
{
    Player *p = find_player(pid);
    if (p == NULL)
        return false;

    return true;
}

void print_players() {
    Player *p;
    Player * head = player_mgr()->players;
    for (p = head; p != NULL; p = (Player *)(p->hh.next)) {
        LOG_INFO("User ID %ld", p->id);
    }
}
