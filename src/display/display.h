#ifndef DISPLAY_H
#define DISPLAY_H

#ifndef DISPLAYTOFONT
#define DISPLAYTOFONT display_tofont
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include <raylib.h>
#include "raynames.h"
#include "list.h"

#define MAX_CHANGES 1024

static Color display_colors[16] = {
    (Color){24, 24, 24, 255},     // 0x0
    (Color){40, 40, 40, 255},     // 0x1
    (Color){56, 56, 56, 255},     // 0x2
    (Color){88, 88, 88, 255},     // 0x3
    (Color){184, 184, 184, 255},  // 0x4
    (Color){216, 216, 216, 255},  // 0x5
    (Color){232, 232, 232, 255},  // 0x6
    (Color){248, 248, 248, 255},  // 0x7
    (Color){171, 70, 66, 255},    // 0x8
    (Color){220, 150, 86, 255},   // 0x9
    (Color){247, 202, 136, 255},  // 0xa
    (Color){161, 181, 108, 255},  // 0xb
    (Color){121, 135, 79, 255},  // 0xc
    (Color){134, 193, 185, 255},  // 0xd
    (Color){124, 175, 194, 255},  // 0xe
    (Color){161, 105, 70, 255}    // 0xf
};

static Texture _font_texture;
static bool _loaded = false;

static int _width;
static int _height;
static int _length;
static uint16_t* _buffer;
static list_t* _stack;
static bool _need_update;
static uint32_t _changes[MAX_CHANGES];
static uint32_t _changes_made = 0;

static RenderTexture2D _framebuffer;
static Shader _post_process;

void display_init(int, int, const char*);
void display_free();
void display_push();
void display_pop();
void display_set_texture(Texture);

void display_print(const char* msg, int x, int y, uint8_t flags);
void display_rich_print(const char* msg, int x, int y);
void display_putc(uint8_t c, int x, int y, uint8_t flags);
void display_clear(uint16_t data);

void display_color_debug(int x, int y);

void display_force_change(int index);
void display_draw();
uint8_t display_tofont(uint8_t ch);

int display_get_width();
int display_get_height();

Vector2 display_get_mouse();

#endif
