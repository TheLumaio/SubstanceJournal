#ifndef DISPLAY_TOOLS_H
#define DISPLAY_TOOLS_H

#include "display.h"

void clear_box(int, int, int, int);
void draw_box(int, int, int, int, uint8_t, uint8_t);
void draw_box_title(int, int, int, int, uint8_t, uint8_t, char*);

#endif
