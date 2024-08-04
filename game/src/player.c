#include "player.h"

#include "logger.h"
#include "utstring.h"

void player_to_string(Player *p)
{
    LOG_INFO("Player: %lu,%s", p->id, p->name);
}