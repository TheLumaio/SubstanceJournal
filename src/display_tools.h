#ifndef DISPLAY_TOOLS_H
#define DISPLAY_TOOLS_H

#include "display.h"

void clear_box(struct Display*, int, int, int, int);
void draw_box(struct Display*, int, int, int, int, uint8_t, uint8_t);
void draw_box_title(struct Display*, int, int, int, int, uint8_t, uint8_t, char*);

#ifdef DISPLAY_TOOLS_IMPL

void clear_box(struct Display* display, int x, int y, int w, int h)
{
    for (int i = 0; i < w+1; i++) {
        for (int j = 0; j < h+1; j++) {
            display_putc(display, 0x00, x+i, y+j, 0x00);
        }
    }
}

void draw_box(struct Display* display, int x, int y, int w, int h, uint8_t outer, uint8_t inner)
{
    display_print(display, "\xc9", x, y, outer);
    display_print(display, "\xbb", x+w, y, outer);
    display_print(display, "\xc8", x, y+h, outer);
    display_print(display, "\xbc", x+w, y+h, outer);
    
    for (int i = 0; i < w-1; i++) {
        display_print(display, "\xcd", x+1+i, y, outer);
        display_print(display, "\xcd", x+1+i, y+h, outer);
    }
    
    for (int i = 0; i < h-1; i++) {
        display_print(display, "\xba", x, y+1+i, outer);
        display_print(display, "\xba", x+w, y+1+i, outer);
    }
    
}

void draw_box_title(struct Display* display, int x, int y, int w, int h, uint8_t outer, uint8_t inner, char* title)
{
    draw_box(display, x, y, w, h, outer, inner);
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
        display_putc(display, title[i], x+i+1, y, 0x07);
    }
    if (b) {
        for (int i = 0; i < 3; i++) {
            display_putc(display, '.', x+w+i+1, y, 0x07);
        }
    }
    
}

#endif

#endif
