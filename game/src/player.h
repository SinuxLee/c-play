#pragma once
#include <stdint.h>
#include <stdio.h>
#include "uthash.h"

typedef struct 
{
    uint64_t id;
    char name[32];
    UT_hash_handle hh; // hash 处理器，必须是这个名字
}Player;

void player_to_string(Player *p);
