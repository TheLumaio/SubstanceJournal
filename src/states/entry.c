#include "state_list.h"
#include "journal.h"

#include <time.h>

static int _scroll = 0;
static int _day = 1;
static int _month = 5;
static int _year = 2020;
static int _selected = -1;

static gui_layout_t* _main;
static gui_layout_t* _settings;
static gui_layout_t* _current;

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

    gui_add_child(_main, gui_button("Add substance", 34, 30, 0, -1, _addsub_click));
    gui_add_child(_main, gui_button("Add entry", 34, 31, 0, -1, _addent_click));
    gui_add_child(_main, gui_calender(37, 4, &_day, &_month, &_year));

    _settings = gui_create();

    // gui_add_child(_main, gui_button("Delete", 3))
}

void entry_enter()
{
    display_clear(0x0000);

    draw_box(0, 1, display_get_width()-1, display_get_height()-2, 0x0c, 0x00);
    display_print("Substance", 1, 1, 0x0d);
    display_print("Amount", 13, 1, 0x0d);
    display_print("Time", 22, 1, 0x0d);
    display_print("Calender", 34, 1, 0x0d);
    display_putc(0xcb, 11, 1, 0x0c);
    display_putc(0xcb, 20, 1, 0x0c);
    display_putc(0xcb, 32, 1, 0x0c);
    display_putc(0xca, 11, display_get_height()-1, 0x0c);
    display_putc(0xca, 20, display_get_height()-1, 0x0c);
    display_putc(0xca, 32, display_get_height()-1, 0x0c);

    for (int i = 0; i < display_get_height()-3; i++) {
        display_putc(0xba, 11, 2+i, 0x01);
        display_putc(0xba, 20, 2+i, 0x01);
        display_putc(0xba, 32, 2+i, 0x0c);
    }

    display_putc(0xcc, 32, 26, 0x0c);
    display_putc(0xb9, 63, 26, 0x0c);
    display_putc(0xcc, 32, 28, 0x0c);
    display_putc(0xb9, 63, 28, 0x0c);
    for (int i = 0; i < 30; i++) {
        display_putc(0xcd, 33+i, 26, 0x0c);
        display_putc(0xcd, 33+i, 28, 0x0c);
    }
    display_print("Note", 34, 26, 0x0d);
    display_print("Settings", 34, 28, 0x0d);
    
    journal_encrypt();

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    _day = tm.tm_mday;
    _month = tm.tm_mon + 1;
    _year = tm.tm_year+1900;


    gui_swap_to(_main);
}

void entry_update()
{
    gui_update(_main);
    
    int mx = display_get_mouse().x;
    int my = display_get_mouse().y;

    list_t* entries = journal_get_entries_at(_day, _month, _year);
    list_t* subs = journal_get_substances();
    char** scales = journal_get_scales();
    if (entries->count == 0) {
        clear_box(1, 2, 9, display_get_height()-4);
        clear_box(12, 2, 7, display_get_height()-4);
        clear_box(21, 2, 10, display_get_height()-4);
    }
    bool hover = false;
    for (int i = _scroll; i < fmin(entries->count, display_get_height()-3); i++) {
        entry_t* ent = entries->data[i];
        substance_t* sub = subs->data[ent->substance];

        uint8_t c = 0x0e;
        if (mx > 0 && mx < 32 && my == 2+i) {
            c = 0x2e;
            if (is_mouse_button_pressed(MOUSE_LEFT_BUTTON)) {
                _selected = i;
                clear_box(33, 27, 29, 0);
            }
            hover = true;
        }

        if (_selected == i) {
            c = 0xce;
        }
        
        display_print(format_text("%s", sub->name), 1, 2+i, c);
        display_print(format_text("%d%s", ent->amount, scales[sub->scale]), 12, 2+i, c);
        display_print(format_text("%d:%d", ent->hour, ent->minute), 21, 2+i, c);
    }

    if (!hover && is_mouse_button_pressed(MOUSE_LEFT_BUTTON)) {
        clear_box(33, 27, 29, 0);
        _selected = -1;
    }

    if (entries->count > 0 && _selected > -1) {
        entry_t* ent = entries->data[_selected];
        display_print(ent->note, 33, 27, 0x07);
    }

    list_empty(entries, false);
    free(entries);
}

bool entry_is_ready()
{
    return _initialized;
}
