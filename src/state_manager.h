#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "display/display.h"

typedef void (*state_init_ptr)();
typedef void (*state_enter_ptr)();
typedef void (*state_update_ptr)();

#define GS(id) id##_init, id##_enter, id##_update
#define GAMESTATE_REG(id) \
    static bool _initialized; \
    void id##_init(); \
    void id##_enter(); \
    void id##_update(); \
    bool id##_is_ready();
#define GAMESTATE_INIT(is) \
    _initialized = is;

#define SM_SET_STATE(id) \
    printf("entering %s...  ", #id); \
    if (!id##_is_ready()) { \
        id##_init(); \
    } \
    sm_set_state(GS(id)); \
    printf("state_enter %s\n", #id);

static state_init_ptr _init_ptr;
static state_enter_ptr _enter_ptr;
static state_update_ptr _update_ptr;

void sm_set_state(state_init_ptr, state_enter_ptr, state_update_ptr);
void sm_init();
void sm_update();

#endif
