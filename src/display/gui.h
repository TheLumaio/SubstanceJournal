#ifndef GUI_H
#define GUI_H

#include <math.h>
#include "display.h"
#include "display_tools.h"
#include "list.h"

typedef struct gui_layout_t gui_layout_t;
typedef struct gui_child_t gui_child_t;

typedef void (*gui_update_ptr)(gui_child_t*);

struct gui_child_t {
    gui_update_ptr update_ptr;
    gui_layout_t* layout;
    bool pre_draw;
    bool has_focus;
    void* data;
    int x, y, width, height;
};

struct gui_layout_t {
    list_t* children;
};

gui_layout_t* gui_create();
gui_child_t* gui_add_child(gui_layout_t*, gui_child_t*);
void gui_update(gui_layout_t*);
void gui_request_focus(gui_child_t*);
void gui_swap_to(gui_layout_t*);

// test
typedef void(*on_click_ptr)();
gui_child_t* gui_button(char*, int, int, int, int8_t, on_click_ptr);
static void gui_button_update(gui_child_t*);

typedef void(*on_text_changed_ptr)(char*, int);
gui_child_t* gui_text_input(char*, int, int, int, int, bool, char*);
static void gui_text_input_update(gui_child_t*);

gui_child_t* gui_drop_menu(char*, char**, int, int, int, int, int*);
static void gui_drop_menu_update(gui_child_t*);

#endif
