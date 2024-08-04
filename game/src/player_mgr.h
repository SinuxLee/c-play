#pragma once
#include <stdbool.h>

#include "player.h"

Player *create_player(uint64_t pid);

Player *find_player(uint64_t);

bool remove_player(Player *);

bool update_player(uint64_t, const char *);

void print_players();
