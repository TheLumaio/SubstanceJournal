#include "state_list.h"
#include "journal.h"

static char _name[17];
static int _scale = 0;

static gui_layout_t* _main;

static void _create_click()
{
    substance_t* sub = malloc(sizeof(substance_t));
    strcpy(sub->name, _name);
    sub->scale = _scale;
    sub->uid = rand();

    journal_add_substance(sub);

    SM_SET_STATE(entry);
}

static void _cancel_click()
{
    SM_SET_STATE(entry);
}

void substance_init()
{
    GAMESTATE_INIT(true);

    _main = gui_create();
    gui_add_child(_main, gui_text_input("Substance name", display_get_width()/2-26/2, 15, 18, 16, false, _name));
    gui_add_child(_main, gui_drop_menu("Scale", journal_get_scales(), 4, display_get_width()/2+6, 15, 8, &_scale));
    gui_add_child(_main, gui_button("Create", display_get_width()/2-26/2+1, 18, 0, 0x0c, _create_click));
    gui_add_child(_main, gui_button("Cancel", display_get_width()/2-26/2+12, 18, 0, 0x08, _cancel_click));
}

void substance_enter()
{
    display_clear(0x0000);
    const char* title = \
"            _      _    _              "\
"           /_\\  __| |__| |             "\
"          / _ \\/ _` / _` |             "\
" ___     /_/ \\_\\__,_\\__,_|             "\
"/ __|_  _| |__ __| |_ __ _ _ _  __ ___ "\
"\\__ \\ || | '_ (_-<  _/ _` | ' \\/ _/ -_)"\
"|___/\\_,_|_.__/__/\\__\\__,_|_||_\\__\\___|";
    int y = 0;
    for (int i = 0; i < strlen(title); i++) {
        display_putc(title[i], 13+i%39, 2+y, 0x0c);
        if ((i+1)%39 == 0) y++;
    }

    memset(_name, 0, 17);

    gui_swap_to(_main);
}

void substance_update()
{
    gui_update(_main);
}

bool substance_is_ready()
{
    return _initialized;
}
