#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#include "raynames.h"

#include "display.h"
#define DISPLAY_TOOLS_IMPL
#include "display_tools.h"

#include "journal.h"

int main(int argc, char** argv)
{
    set_trace_log_level(5);
    set_trace_log_exit(5);
    init_window(640, 480, "SubstanceJournal");
    set_exit_key(KEY_F12);
    
    display_load("font.png");
    struct Display* display = display_create(64, 48);
    
    jmenu_draw_offline(display);
    
    while (!window_should_close())
    {
        jmenu_ptr update = jmenu_get_ptr();
        update(display);
        
        begin_drawing();
        clear_background(BLACK);
        display_draw(display);
        end_drawing();
        
        if (is_key_pressed(KEY_Q) & is_key_down(KEY_LEFT_CONTROL)) {
            break;
        }
    }
    close_window();
    
    display_unload();
    free(display);
    jmenu_free();
    
    
    return 0;
}
