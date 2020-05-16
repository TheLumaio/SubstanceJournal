
#include "display_tools.h"

void clear_box(int x, int y, int w, int h)
{
    for (int i = 0; i < w+1; i++) {
        for (int j = 0; j < h+1; j++) {
            display_putc(0x00, x+i, y+j, 0x00);
        }
    }
}

void draw_box(int x, int y, int w, int h, uint8_t outer, uint8_t inner)
{
    display_print("\xc9", x, y, outer);
    display_print("\xbb", x+w, y, outer);
    display_print("\xc8", x, y+h, outer);
    display_print("\xbc", x+w, y+h, outer);
    
    for (int i = 0; i < w-1; i++) {
        display_print("\xcd", x+1+i, y, outer);
        display_print("\xcd", x+1+i, y+h, outer);
    }
    
    for (int i = 0; i < h-1; i++) {
        display_print("\xba", x, y+1+i, outer);
        display_print("\xba", x+w, y+1+i, outer);
    }
    
}

void draw_box_title(int x, int y, int w, int h, uint8_t outer, uint8_t inner, char* title)
{
    draw_box(x, y, w, h, outer, inner);
    bool b = false;
    int len = strlen(title);
    if (len > w-2) {
        w = w-4;
        b = true;
    } else {
        w = len;
    }
    for (int i = 0; i < w; i++)
    {
        display_putc(title[i], x+i+1, y, 0x07);
    }
    if (b) {
        for (int i = 0; i < 3; i++) {
            display_putc('.', x+w+i+1, y, 0x07);
        }
    }
    
}
