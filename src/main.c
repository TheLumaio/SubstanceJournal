#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#include "raynames.h"

#include "display/display.h"
#include "display/display_tools.h"

#include "state_manager.h"
#include "states/state_list.h"

#include "taffer.h"

int main(int argc, char** argv)
{
    // set_trace_log_level(5);
    // set_trace_log_exit(5);
    init_window(640, 480, "SubstanceJournal");
    set_exit_key(KEY_F12);
    
    display_init(64, 48, "Taffer_10x10.png");

    Image i = load_image_pro((void*)taffer, 160, 160, 7);
    Texture texture = load_texture_from_image(i);
    unload_image(i);
    display_set_texture(texture);
    
    sm_init();
    SM_SET_STATE(login);
    
    while (!window_should_close())
    {
        
        sm_update();
        
        begin_drawing();
        clear_background(BLACK);
        display_draw();
        end_drawing();
        
        if (is_key_pressed(KEY_Q) && is_key_down(KEY_LEFT_CONTROL)) {
            break;
        }
    }
    close_window();
    
    
    return 0;
}
