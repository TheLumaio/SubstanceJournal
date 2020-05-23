#include "state_list.h"

#include "journal.h"

static gui_layout_t* _main;
static int _sub_selected = 0;

static list_t* _sub_list;

void dose_init()
{
    GAMESTATE_INIT(true);

    _sub_list = list_create(8);

    _main = gui_create();
    gui_add_child(_main, gui_drop_menu("Substance", (char**)_sub_list->data, _sub_list->count, 20, 30, 16, &_sub_selected));
}

void dose_enter()
{
    display_clear(0x0000);
    const char* title = \
    " ___              "\
    "|   \\ ___ ___ ___ "\
    "| |) / _ (_-</ -_)"\
    "|___/\\___/__/\\___|";
    int y = 0;
    for (int i = 0; i < strlen(title); i++) {
        display_putc(title[i], 23+i%18, 8+y, 0x0c);
        if ((i+1)%18 == 0) y++;
    }

    list_empty(_main->children, true);

    list_t* subs = journal_get_substances();
    list_empty(_sub_list, true);
    for (int i = 0; i < subs->count; i++) {
        char* s = malloc(sizeof(char)*17);
        substance_t* u = subs->data[i];
        strcpy(s, u->name);
        list_append(_sub_list, s);
    }

    gui_add_child(_main, gui_drop_menu("Substance", (char**)_sub_list->data, _sub_list->count, 20, 18, 16, &_sub_selected));
    
    gui_swap_to(_main);
}

void dose_update()
{
    gui_update(_main);
}

bool dose_is_ready()
{
    return _initialized;
}
