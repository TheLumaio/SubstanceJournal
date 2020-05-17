#include "display.h"

#define X(x) #x

void display_init(int width, int height, const char* font_file)
{
    _font_texture = load_texture(font_file);
    _framebuffer = load_render_texture(width*10, height*10);
    
    // _post_process = load_shader(NULL, "post_process.glsl");
    
    memset(_changes, 0, sizeof(uint32_t)*MAX_CHANGES);
    
    _width = width;
    _height = height;
    _length = width*height;
    _need_update = false;
    
    _buffer = malloc(sizeof(uint16_t)*_length);
    memset(_buffer, 0xaa10, sizeof(uint16_t)*_length);

    _stack = list_create(8);
    
    for (int i = 0; i < _length; i++) {
        if (_changes_made == MAX_CHANGES) {
            display_draw();
        }
        _changes[_changes_made++] = i;
    }
    // TODO: shit's broken. why the fuck do i need this
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            display_putc(0x00, i, j, 0x00);
        }
    }

    // display_force_change(0);
    display_draw();

    _loaded = true;
}

void display_free()
{
    unload_texture(_font_texture);
    unload_render_texture(_framebuffer);

    free(_buffer);
    // free(display);
}

void display_push()
{
    uint16_t* stack_buffer = malloc(sizeof(uint16_t)*_length);
    memcpy(stack_buffer, _buffer, sizeof(uint16_t)*_length);
    list_append(_stack, stack_buffer);
}

void display_pop()
{
    memcpy(_buffer, _stack->data[_stack->count-1], sizeof(uint16_t)*_length);
    list_remove(_stack, _stack->count-1, true);
    for (int i = 0; i < _length; i++) {
        display_force_change(i);
    }
}

void display_set_texture(Texture t)
{
    _font_texture = t;
}

void display_print(const char* msg, int x, int y, uint8_t flags)
{

    for (int i = 0; i < strlen(msg); i++) {
        display_putc(msg[i], x+i, y, flags);
    }

    // pointlessly verbose?

    // for (int i = 0; i < strlen(msg); i++)
    // {
    //     uint16_t fgbg = flags << 8;
    //     _buffer[(x+i)*_height+y] = fgbg | DISPLAYTOFONT(msg[i]);
    //     if (_changes_made == MAX_CHANGES) {
            // display_draw();
    //     }
    //     _changes[_changes_made++] = (x+i)*_height+y;
    // }
    // _need_update = true;
}

void display_rich_print(const char* msg, int x, int y)
{
    int len = strlen(msg);
    char color_buffer[len];
    int delta = 0;
    for (int i = 0; i < len; i++)
    {
        if (msg[i] == '{') {
            memset(color_buffer, 0, len);
            i++;
            while (msg[i] != '}') {
                color_buffer[strlen(color_buffer)] = msg[i];
                i++;
            }
        }
        else if (msg[i] == '\n') {
            delta = 0;
            y++;
        }
        else {
            display_putc(msg[i], x+delta, y, strtol(color_buffer, NULL, 0));
            delta++;
        }
    }
}

void display_putc(uint8_t c, int x, int y, uint8_t flags)
{
    uint16_t fgbg = flags << 8;
    int index = x*_height+y;
    if (x < 0 || y < 0 || x > _width-1 || y > _height-1) {
        return;
    }
    
    if (_buffer[index] == (fgbg | DISPLAYTOFONT(c))) return; // +200-300 fps lmao
    _buffer[index] = fgbg | DISPLAYTOFONT(c);
    if (_changes_made == MAX_CHANGES) {
        display_draw();
    }
    _changes[_changes_made++] = index;
}

void display_clear(uint16_t data)
{
    _changes_made = 0;
    memset(_buffer, data, sizeof(uint16_t) * _length);
    for (int i = 0; i < _length; i++) {
        display_force_change(i);
    }
    display_draw();
}

void display_color_debug(int x, int y)
{
    for (int i = 0; i < 16; i++) {
        display_putc(0xdb, x, y+i, i);
        display_rich_print(FormatText("{0x07}0x%x", i), x+3, y+i);
    }
}

void display_force_change(int index)
{
    if (_changes_made == MAX_CHANGES) {
        display_draw();
    }
    _changes[_changes_made++] = index;
}

void display_draw()
{
    draw_text("fuck", 0, 0, 20, RED);
    if (_changes_made > 0) {
        begin_texture_mode(_framebuffer);
        
        for (int i = 0; i < _changes_made; i++)
        {
            int j = _changes[i];
            
            int _x = j / _height;
            int _y = j % _height;
            
            int ch = _buffer[j]&0x00ff;
            int fg = (_buffer[j]&0x0f00)>>8;
            
            int _fx = ch % 16;
            int _fy = ch / 16;
            
            draw_rectangle(_x*10, _y*10, 10, 10, display_colors[(_buffer[j]&0xf000)>>12]);
            draw_texture_rec(_font_texture, (Rectangle){_fx*10, _fy*10, 10, 10}, (Vector2){_x*10, _y*10}, display_colors[fg]);
        }
        _changes_made = 0;
        
        end_texture_mode();
        _need_update = false;
    }
    
    // begin_shader_mode(post_process);
    // draw_texture_rec(_framebuffer.texture, (Rectangle){0, 0, _framebuffer.texture.width, -_framebuffer.texture.height}, (Vector2){0, 0}, WHITE);
    draw_texture_pro(_framebuffer.texture, (Rectangle){0, 0, _framebuffer.texture.width, -_framebuffer.texture.height}, (Rectangle){0, 0, get_screen_width(), get_screen_height()}, (Vector2){0, 0}, 0, WHITE);
    // end_shader_mode();
}

uint8_t display_tofont(uint8_t ch)
{
    // return ch;
    if (ch >= 'A' && ch <= 'Z') {
        return 0x0041+(ch-'A');
    }
    if (ch >= 'a' && ch <= 'z') {
        return 0x0061+(ch-'a');
    }
    if (ch == ' ') return 0x0000;
    if (ch == '[') return 0x005b;
    if (ch == '\\') return 0x005c;
    if (ch == ']') return 0x005d;
    else return ch;
}

int display_get_width() { return _width; }
int display_get_height() { return _height; }

Vector2 display_get_mouse()
{
    static Vector2 pos;
    pos.x = floor((get_mouse_x()*((float)_framebuffer.texture.width/(float)get_screen_width()))/10);
    pos.y = floor((get_mouse_y()*((float)_framebuffer.texture.height/(float)get_screen_height()))/10);
    return pos;
}
