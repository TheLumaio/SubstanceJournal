#include "state_list.h"

void dose_init()
{
    GAMESTATE_INIT(true);
}

void dose_enter()
{
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
    
    draw_box_title(display_get_width()/2-31, 8, 11, 36, 0x0c, 0x00, "Substance");
    draw_box_title(display_get_width()/2-19, 8, 4, 36, 0x0c, 0x00, "ID");
    
    // for (int i = 0; i < _header->substance_count; i++) {
    //     display_print(_substances[i].name, display_get_width()/2-30, 9+i, 0x0a);
    //     display_print(format_text("%u", _substances[i].uid), display_get_width()/2-18, 9+i, 0x0a);
    // }
    
    draw_box_title(display_get_width()/2-26/2, 18, 26, 2, 0x0c, 0x00, "Command");
    
    display_rich_print("{0x08}Enter {0x07}to add dosage", display_get_width()/2-26/2+4, 23);
    display_rich_print("{0x08}Escape {0x07}to cancel", display_get_width()/2-26/2+5, 25);
}

void dose_update()
{

}

bool dose_is_ready()
{
    return _initialized;
}
