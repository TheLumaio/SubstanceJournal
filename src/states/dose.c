#include "state_list.h"
#include <string.h>
#include <time.h>

#include "journal.h"

static gui_layout_t* _main;
static int _sub_selected = 0;
static list_t* _sub_list;

static char _amount[9];
static char _comments[33];

static void _done_click()
{
    entry_t* entry = malloc(sizeof(entry_t));

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    entry->day = tm.tm_mday;
    entry->month = tm.tm_mon + 1;
    entry->year = (1900+tm.tm_year)-2000;

    entry->hour = tm.tm_hour;
    entry->minute = tm.tm_min;

    entry->amount = atoi(_amount);
    entry->substance = _sub_selected;

    strcpy(entry->note, _comments);

    journal_add_entry(entry);

    memset(_amount, 0, sizeof(_amount));
    memset(_comments, 0, sizeof(_comments));

    SM_SET_STATE(entry);

}

static void _cancel_click()
{
    memset(_amount, 0, sizeof(_amount));
    memset(_comments, 0, sizeof(_comments));

    SM_SET_STATE(entry);
}

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

    gui_add_child(_main, gui_drop_menu("Substance", (char**)_sub_list->data, _sub_list->count, display_get_width()/2-9, 18, 17, &_sub_selected));
    gui_add_child(_main, gui_text_input("Amount", display_get_width()/2-9, 22, 17, 8, false, _amount));
    gui_add_child(_main, gui_text_input("Comments", display_get_width()/2-9, 26, 17, 32, false, _comments));
    
    gui_add_child(_main, gui_button("Done", display_get_width()/2-9, 30, 0, 0x0c, _done_click));
    gui_add_child(_main, gui_button("Cancel", display_get_width()/2+3, 30, 0, 0x08, _cancel_click));
    
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
