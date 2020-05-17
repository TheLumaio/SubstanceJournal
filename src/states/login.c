#include "state_list.h"
#include "journal.h"

static gui_layout_t* _main;
static char _password[17];

static void _password_changed(char* t, int l)
{
    memcpy(_password, t, l);
}

static void _login_click()
{
    journal_decrypt(_password);
}

static void _create_click()
{
    journal_new(_password);
    
    printf("%s\n", _password);
    memset(_password, 0, 20);

    // reset the text input to empty. this is awful.
    // clear_box(display_get_width()/2-26/2, 18, 26, 2);
    // list_remove(_main->children, _main->children->entries-1, true);
    // gui_child_t* c = gui_add_child(_main, gui_text_input("Password", display_get_width()/2-26/2, 18, 26, 16, true, _password));
    // c->has_focus = true;

}

void login_init()
{
    GAMESTATE_INIT(true);

    memset(_password, 0, 17);

    _main = gui_create();
}

void login_enter()
{
    const char* title = \
    " _              _      " \
    "| |   ___  __ _(_)_ _  " \
    "| |__/ _ \\/ _` | | ' \\ " \
    "|____\\___/\\__, |_|_||_|" \
    "          |___/        ";
    int y = 0;
    for (int i = 0; i < strlen(title); i++) {
        display_putc(title[i], 20+i%23, 8+y, 0x0c);
        if ((i+1)%23 == 0) y++;
    }
    
    // display_print("F5", display_get_width()/2-26/2-2, 25, 0x08);
    // display_print("to generate offline database", display_get_width()/2-26/2+1, 25, 0x07);
    
    // draw_box_title(display_get_width()/2-26/2, 18, 26, 2, 0x0c, 0x00, "Password");
    gui_add_child(_main, gui_button("Login", display_get_width()/2-26/2+1, 22, 0, 0x0c, _login_click));
    gui_add_child(_main, gui_button("Create", display_get_width()/2+26/2-6, 22, 0, 0x08, _create_click));
    gui_add_child(_main, gui_text_input("Password", display_get_width()/2-26/2, 18, 26, 16, true, _password));
    
    display_rich_print("{0x09}SubstanceJournal {0x07}by {0x08}.xnc {0x03}based on {0x09}dJournal {0x07}by {0x08}boomer678", 1, 46);
}

void login_update()
{
    gui_update(_main);
}

bool login_is_ready()
{
    return _initialized;
}
