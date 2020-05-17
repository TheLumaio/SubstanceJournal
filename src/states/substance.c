#include "state_list.h"

void substance_init()
{
    GAMESTATE_INIT(true);
}

void substance_enter()
{
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

    draw_box_title(display_get_width()/2-26/2, 15, 26, 2, 0x0c, 0x00, "Substance name");
    
    display_rich_print("{0x08}Enter {0x07}to add substance", display_get_width()/2-26/2+2, 23);
    display_rich_print("{0x08}Escape {0x07}to cancel", display_get_width()/2-26/2+5, 25);
}

void substance_update()
{

}

bool substance_is_ready()
{
    return _initialized;
}
