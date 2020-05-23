#include "state_list.h"
#include "journal.h"

static int _scroll = 0;
static int _day = 1;
static int _month = 5;
static int _year = 2020;

static gui_layout_t* _main;

static void _addsub_click()
{
    SM_SET_STATE(substance);
}

static void _addent_click()
{
    SM_SET_STATE(dose);
}

void entry_init()
{
    GAMESTATE_INIT(true);

    _main = gui_create();

    gui_add_child(_main, gui_button("Add substance", 30, 40, 0, -1, _addsub_click));
    gui_add_child(_main, gui_button("Add entry", 30, 41, 0, -1, _addent_click));
    gui_add_child(_main, gui_calender(34, 4, &_day, &_month, &_year));
}

void entry_enter()
{
    display_clear(0x0000);

    draw_box(0, 1, display_get_width()-1, display_get_height()-2, 0x0c, 0x00);
    display_print("Substance", 1, 1, 0x0d);
    display_print("Amount", 13, 1, 0x0d);
    display_print("Date", 22, 1, 0x0d);
    display_putc(0xcb, 11, 1, 0x0c);
    display_putc(0xcb, 20, 1, 0x0c);
    display_putc(0xcb, 27, 1, 0x0c);
    display_putc(0xca, 11, display_get_height()-1, 0x0c);
    display_putc(0xca, 20, display_get_height()-1, 0x0c);
    display_putc(0xca, 27, display_get_height()-1, 0x0c);

    for (int i = 0; i < display_get_height()-3; i++) {
        display_putc(0xba, 11, 2+i, 0x01);
        display_putc(0xba, 20, 2+i, 0x01);
        display_putc(0xba, 27, 2+i, 0x0c);
    }

    list_t* entries = journal_get_entries();
    for (int i = _scroll; i < fmin(entries->count, display_get_height()-3); i++) {
        
    }

    gui_swap_to(_main);
}

void entry_update()
{
    gui_update(_main);
}

bool entry_is_ready()
{
    return _initialized;
}
