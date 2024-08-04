#include "global.h"

// 单例
Global* get_global() {
    static Global instance = {.value = 100};
    return &instance;
}
