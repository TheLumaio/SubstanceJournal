#include "state_manager.h"

void sm_set_state(state_init_ptr init, state_enter_ptr enter, state_update_ptr update)
{
    _init_ptr = init;
    _enter_ptr = enter;
    _update_ptr = update;

    _enter_ptr();
}

void sm_init()
{
    
}

void sm_update()
{
    _update_ptr();
}
