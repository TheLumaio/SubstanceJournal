#include "gui.h"


gui_layout_t* gui_create()
{
    gui_layout_t* layout = malloc(sizeof(gui_layout_t));
    layout->children = list_create(8);
    return layout;
}

gui_child_t* gui_add_child(gui_layout_t* layout, gui_child_t* child)
{
    child->layout = layout;
    gui_request_focus(child);
    list_append(layout->children, child);
    return child;
}

void gui_update(gui_layout_t* layout)
{
    int mx, my;
    mx = display_get_mouse().x;
    my = display_get_mouse().y;

    for (int i = 0; i < layout->children->entries; i++) {
        gui_child_t* child = layout->children->data[i];

        if (is_mouse_button_pressed(MOUSE_LEFT_BUTTON)) {
            if (mx >= child->x && mx < child->x+child->width && my >= child->y && my < child->y+child->height) {
                    gui_request_focus(child);
            } else {
                child->has_focus = false;
            }
        }

        gui_update_ptr ptr = child->update_ptr;
        ptr(child);
    }
}

void gui_request_focus(gui_child_t* child)
{
    // if (child->has_focus) return;
    gui_layout_t* layout = child->layout;
    for (int i = 0; i < layout->children->entries; i++) {
        gui_child_t* nchild = layout->children->data[i];
        if (nchild != child)
            nchild->has_focus = false;
    }
    child->has_focus = true;
    child->pre_draw = true;
}

void gui_swap_to(gui_layout_t* layout)
{
    for (int i = 0; i < layout->children->entries; i++) {
        gui_child_t* nchild = layout->children->data[i];
        nchild->pre_draw = true;
    }
}

// Gui tools

typedef struct button_info_t {
    char* text;
    bool hover;
    on_click_ptr callback;
    uint8_t tint;
} button_info_t;

gui_child_t* gui_button(char* text, int x, int y, int w, int8_t tint, on_click_ptr ptr)
{
    gui_child_t* child = malloc(sizeof(gui_child_t));
    child->update_ptr = gui_button_update;
    child->pre_draw = true;
    child->x = x;
    child->y = y;
    child->width = fmax(strlen(text), w);
    child->height = 1;

    button_info_t* button_info = malloc(sizeof(button_info_t));
    button_info->text= text;
    button_info->hover = false;
    button_info->callback = ptr;
    button_info->tint = (tint==-1)?0x07:tint;

    child->data = button_info;

    return child;
}

void gui_button_update(gui_child_t* child)
{
    button_info_t* button_info = child->data;

    if (child->pre_draw) {
        display_print(button_info->text, child->x, child->y, button_info->tint);
        child->pre_draw = false;
    }

    // mouse to display coords
    int mx, my;
    mx = display_get_mouse().x;
    my = display_get_mouse().y;

    if (mx >= child->x && mx < child->x + child->width && my == child->y)
    {
        if (!button_info->hover) {
            button_info->hover = true;
            display_print(button_info->text, child->x, child->y, (button_info->tint>>4)|(button_info->tint<<4));
        }
        if (is_mouse_button_pressed(MOUSE_LEFT_BUTTON) && button_info->callback != NULL) {
            button_info->callback();
        }
    } else if (button_info->hover) {
        display_print(button_info->text, child->x, child->y, button_info->tint);
        button_info->hover = false;
    }

}

typedef struct text_input_t {
    char* title;
    char* text;
    int size;
    int max_size;
    int cursor_x;
    int offset;
    bool gained_focus;
    bool is_pass;
    on_text_changed_ptr text_changed;
} text_input_t;

gui_child_t* gui_text_input(char* title, int x, int y, int width, int max, bool is_pass, on_text_changed_ptr ptr)
{
    gui_child_t* child = malloc(sizeof(gui_child_t));
    child->update_ptr = gui_text_input_update;
    child->pre_draw = true;
    child->x = x;
    child->y = y;
    child->width = width;
    child->height = 2;

    text_input_t* text_input = malloc(sizeof(text_input_t));
    text_input->title = title;
    text_input->text = malloc(sizeof(char) * max);
    text_input->size = 0;
    text_input->max_size = max;
    text_input->cursor_x = 0;
    text_input->offset = 0;
    text_input->gained_focus = false;
    text_input->is_pass = is_pass;
    text_input->text_changed = ptr;

    child->data = text_input;

    return child;
}

void gui_text_input_update(gui_child_t* child)
{
    text_input_t* text_input = child->data;

    if (child->pre_draw) {
        draw_box_title(child->x, child->y, child->width, 2, 0x04, 0x00, text_input->title);
        child->pre_draw = false;
    }

    if (child->has_focus && !text_input->gained_focus) {
        display_putc(0xdb, child->x+1+text_input->cursor_x-text_input->offset, child->y+1, 0x07);
        text_input->gained_focus = true;
    }
    if (!child->has_focus && text_input->gained_focus) {
        display_putc(0x00, child->x+1+text_input->cursor_x-text_input->offset, child->y+1, 0x00);
        text_input->gained_focus = false;
    }

    if (!child->has_focus) return;

    int c = get_key_pressed();
    if (c > 0)
    {
        if (text_input->size < text_input->max_size) {
            text_input->text[text_input->cursor_x] = c;
            text_input->size++;
            text_input->cursor_x++;
            if (text_input->size >= child->width-1) {
                text_input->offset++;
            }
            display_putc(0xdb, child->x+1+text_input->cursor_x-text_input->offset, child->y+1, 0x07);
            for (int i = text_input->offset; i < text_input->size; i++) {
                uint8_t c = 0x07;
                if (i == text_input->cursor_x)
                    c = 0x70;
                if (text_input->is_pass)
                    display_putc('*', child->x+1+i-text_input->offset, child->y+1, i%15+1);
                else
                    display_putc(text_input->text[i], child->x+1+i-text_input->offset, child->y+1, c);
            }

            text_input->text_changed(text_input->text, text_input->size);
        }
    }

    if (is_key_pressed(KEY_BACKSPACE) && text_input->size > 0)
    {
        text_input->text[text_input->size-1] = '\0';
        text_input->size--;

        if (text_input->cursor_x-text_input->offset > 1)
        {
            display_putc(0x00, child->x+1+text_input->cursor_x-text_input->offset, child->y+1, 0x00);
            text_input->cursor_x--;
            display_putc(0xdb, child->x+1+text_input->cursor_x-text_input->offset, child->y+1, 0x07);
        } else {
            display_putc(0x00, child->x+1+text_input->cursor_x-text_input->offset, child->y+1, 0x00);
            text_input->offset = fmax(0, text_input->offset-child->width+2);
            text_input->cursor_x = text_input->size;
            display_putc(0xdb, child->x+1+text_input->cursor_x-text_input->offset, child->y+1, 0x07);
        }
        for (int i = text_input->offset; i < text_input->size; i++) {
            uint8_t c = 0x07;
            if (i == text_input->cursor_x)
                c = 0x70;
            if (text_input->is_pass)
                display_putc('*', child->x+1+i-text_input->offset, child->y+1, i%15+1);
            else
                display_putc(text_input->text[i], child->x+1+i-text_input->offset, child->y+1, c);
        }

        text_input->text_changed(text_input->text, text_input->size);
    }

}

typedef struct drop_menu_t {
    char* title;
    char** list;
    int list_length;
    int* selected;
    bool hover;
    bool drop;
} drop_menu_t;

gui_child_t* gui_drop_menu(char* title, char** list, int list_length, int x, int y, int width, int* selected)
{
    gui_child_t* child = malloc(sizeof(gui_child_t));
    child->update_ptr = gui_drop_menu_update;
    child->pre_draw = true;
    child->x = x;
    child->y = y;
    child->width = 0;
    child->height = 2;

    int max_width = 0;
    for (int i = 0; i < list_length; i++) {
        int l = strlen(list[i])+2;
        if (l > max_width) max_width = l;
    }
    child->width = fmax(max_width, width);

    drop_menu_t* drop_menu = malloc(sizeof(drop_menu_t));
    drop_menu->title = title;
    drop_menu->list = list;
    drop_menu->list_length = list_length;
    drop_menu->selected = selected;
    drop_menu->hover = false;
    drop_menu->drop = false;

    child->data = drop_menu;

    return child;
}

void gui_drop_menu_update(gui_child_t* child)
{
    static drop_menu_t* open_menu = NULL;

    drop_menu_t* drop_menu = child->data;

    if (child->pre_draw) {
        clear_box(child->x, child->y, child->width, 2);
        display_putc('\xc9', child->x, child->y, 0x07);
        display_putc('\xbb', child->x+child->width, child->y, 0x07);
        display_putc('\xc8', child->x, child->y+1, 0x07);
        display_putc('\xbc', child->x+child->width, child->y+1, 0x07);
        for (int i = 0; i < child->width-1; i++) {
            display_putc('\xcd', child->x+1+i, child->y, 0x07);
        }

        display_print(drop_menu->list[*drop_menu->selected], child->x+1, child->y+1, 0x0d);

        child->pre_draw = false;
    }

    int mx = display_get_mouse().x;
    int my = display_get_mouse().y;

    if (drop_menu->drop)
    {
        for (int i = 0; i < drop_menu->list_length; i++)
        {
            if (mx >= child->x+1 && mx < child->x + child->width && my == child->y+2+i)
            {
                if (is_mouse_button_pressed(MOUSE_LEFT_BUTTON))
                {
                    *drop_menu->selected = i;
                    open_menu = NULL;
                    drop_menu->drop = false;
                    child->pre_draw = true;
                    printf("drop select %d\n", *drop_menu->selected);
                    display_pop();
                    break;
                }

                display_print(drop_menu->list[i], child->x+1, child->y+2+i, 0x40);
            } else {
                display_print(drop_menu->list[i], child->x+1, child->y+2+i, 0x04);
            }
        }
    }

    if (mx >= child->x+1 && mx < child->x + child->width && my == child->y+1)
    {
        if (!drop_menu->hover) {
            drop_menu->hover = true;
            display_print(drop_menu->list[*drop_menu->selected], child->x+1, child->y+1, 0xd0);
        }
        if (is_mouse_button_pressed(MOUSE_LEFT_BUTTON))
        {
            // open/close menu
            if (open_menu)
            {
                open_menu->drop = false;
                open_menu = NULL;
                display_pop();
                open_menu = drop_menu;
                drop_menu->drop = true;
                child->pre_draw = true;
                display_push();
            }
            else
            {
                open_menu = drop_menu;
                drop_menu->drop = true;
                child->pre_draw = true;
                display_push();
            }

        }
    } else
    {
        if (drop_menu->hover) {
            display_print(drop_menu->list[*drop_menu->selected], child->x+1, child->y+1, 0x0d);
            drop_menu->hover = false;
        }

        if (is_mouse_button_pressed(MOUSE_LEFT_BUTTON) && drop_menu->drop)
        {
            if (open_menu)
            {
                open_menu->drop = false;
                open_menu = NULL;
                child->pre_draw = true;
                display_pop();
            }
        }
    }


}
