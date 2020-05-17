#include "state_list.h"

static int _scroll = 0;

void entry_init()
{
    GAMESTATE_INIT(true);
}

void entry_enter()
{
    
}

void entry_update()
{

}

bool entry_is_ready()
{
    return _initialized;
}
