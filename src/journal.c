#include "journal.h"

static bool isch(char ch) {
    static const char* alphabet = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-=!@#$%^&*()_+`~[]{}|\\,./<>?;':\"";
    for (int i = 0; i < 96; i++) {
        if (alphabet[i] == ch)
            return true;
    }
    return false;
}

void jmenu_init()
{
    
}

static void jmenu_dump()
{
    for (int i = 0; i < _header->entry_count; i++) {
        struct entry_t* e = &_entries[i];
        printf("- entry\n-- %d\n-- %d\n-- %s\n-- %s\n-- %s\n", e->substance_index, e->dose, e->scale, e->day, e->date);
    }
    for (int i = 0; i < _header->substance_count; i++) {
        struct substance_t* s = &_substances[i];
        printf("- substance\n-- %s\n", s->name);
    }
}

static void jmenu_encrypt(char* password)
{
    AES_init_ctx_iv(&_ctx, password, "thisisnot16bytes");
    
    size_t header_size = sizeof(struct header_t);
    size_t entries_size = sizeof(struct entry_t) * _header->entry_count;
    size_t substances_size = sizeof(struct substance_t) * _header->substance_count;
    
    struct header_t* header_enc = malloc(sizeof(struct header_t));
    struct entry_t* entries_enc = malloc(entries_size);
    struct substance_t* substances_enc = malloc(substances_size);
    
    memcpy(header_enc, _header, sizeof(struct header_t));
    memcpy(entries_enc, _entries, entries_size);
    memcpy(substances_enc, _substances, substances_size);
    
    AES_CBC_encrypt_buffer(&_ctx, (uint8_t*)header_enc, header_size);
    AES_CBC_encrypt_buffer(&_ctx, (uint8_t*)entries_enc, entries_size);
    AES_CBC_encrypt_buffer(&_ctx, (uint8_t*)substances_enc, substances_size);
    
    FILE* f = fopen("data.bin", "wb");
    fwrite(header_enc, header_size, 1, f);
    fwrite(entries_enc, entries_size, 1, f);
    fwrite(substances_enc, substances_size, 1, f);
    fclose(f);
    
    free(header_enc);
    free(entries_enc);
    free(substances_enc);
    
}

static void jmenu_decrypt(char* password)
{
    AES_init_ctx_iv(&_ctx, password, "thisisnot16bytes");
    
    // read buffer
    FILE* f = fopen("data.bin", "rb");
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    void* data = malloc(fsize);
    fread(data, fsize, 1, f);
    
    // decrypt
    AES_CBC_decrypt_buffer(&_ctx, data, fsize);
    
    // unpack binary data
    _header = malloc(sizeof(struct header_t));
    memcpy(_header, data, sizeof(struct header_t));
    
    size_t entries_size = sizeof(struct entry_t) * _header->entry_count;
    size_t substances_size = sizeof(struct substance_t) * _header->substance_count;
    
    if (strncmp(_header->seq, "SJ01", 4)) {
        fclose(f);
        free(data);
        return;
    }
    
    _entries = malloc(entries_size);
    memcpy(_entries, data+sizeof(struct header_t), entries_size);
    
    _substances = malloc(substances_size);
    memcpy(_substances, data+sizeof(struct header_t)+entries_size, substances_size);
    
    // clean up
    fclose(f);
    free(data);
}

void jmenu_draw_dosages(struct Display* display)
{
    jmenu_update = jmenu_update_dosages;
    
    jmenu_dump();
    
    draw_box_title(display, 1, 1, 43, 45, 0x0c, 0x00, "List");
    draw_box_title(display, 2, 2, 11, 43, 0x0c, 0x00, "Substance");
    draw_box_title(display, 14, 2, 7, 43, 0x0c, 0x00, "Dose");
    draw_box_title(display, 22, 2, 7, 43, 0x0c, 0x00, "Scale");
    draw_box_title(display, 30, 2, 13, 43, 0x0c, 0x00, "Day");
    draw_box_title(display, 47, 1, 15, 45, 0x0c, 0x00, "Date");
    
    for (int i = 0; i < _header->entry_count; i++) {
        display_print(display, _substances[_entries[i].substance_index].name, 3, 3+i, 0x0a);
        display_print(display, format_text("%u", _entries[i].dose), 15, 3+i, 0x0a);
        display_print(display, _entries[i].scale, 23, 3+i, 0x0a);
        display_print(display, _entries[i].day, 31, 3+i, 0x0a);
        display_print(display, _entries[i].date, 48, 2+i, 0x0a);
    }
    
}

void jmenu_update_dosages(struct Display* display)
{
    if (IsKeyPressed(KEY_S)) {
        clear_box(display, 0, 0, display->width, display->height);
        jmenu_draw_add_substance(display);
        jmenu_update = jmenu_update_add_substance;
    }
}


void jmenu_draw_login(struct Display* display)
{
    
    const char* title =
    "    _                        _ "
    " _ | |___ _  _ _ _ _ _  __ _| |"
    "| || / _ \\ || | '_| ' \\/ _` | |"
    " \\__/\\___/\\_,_|_| |_||_\\__,_|_|";
    int y = 0;
    for (int i = 0; i < strlen(title); i++) {
        display_putc(display, title[i], 17+i%31, 1+y, 0x0c);
        if ((i+1)%31 == 0) y++;
    }
    
    display_print(display, "F5", display->width/2-26/2+7, 25, 0x08);
    display_print(display, "to register", display->width/2-26/2+10, 25, 0x07);
    
    draw_box_title(display, display->width/2-26/2, 15, 26, 2, 0x0c, 0x00, "Username");
    draw_box_title(display, display->width/2-26/2, 20, 26, 2, 0x0c, 0x00, "Password");
    
    display_print(display, "SubstanceJournal", 1, 46, 0x09);
    display_print(display, "by", 18, 46, 0x07);
    display_print(display, ".xnc", 21, 46, 0x08);
    display_print(display, "based on", 26, 46, 0x03);
    display_print(display, "djournal", 35, 46, 0x09);
    display_print(display, "by", 44, 46, 0x07);
    display_print(display, "boomer678", 47, 46, 0x08);
}

void jmenu_update_login(struct Display* display)
{
    static char username[20] = {0};
    static char password[20] = {0};
    static int field = 0;
    int k = get_key_pressed();
    
    switch (field) {
        case 0:
            if (is_key_pressed(KEY_BACKSPACE) && strlen(username) > 0) {
                display_putc(display, 0x00, display->width/2-26/2+1+strlen(username), 16, 0x00);
                username[strlen(username)-1] = 0;
                break;
            }
            if (is_key_pressed(KEY_TAB)) {
                display_putc(display, 0x00, display->width/2-26/2+1+strlen(username), 16, 0x00);
                field = (field + 1) % 2;
                break;
            }
            if (isch(k) && strlen(username) < 20) {
                display_putc(display, k, display->width/2-26/2+1+strlen(username), 16, 0x03);
                username[strlen(username)] = k;
            }
            display_putc(display, 0xdb, display->width/2-26/2+1+strlen(username), 16, 0x07);
            break;
        case 1:
            if (is_key_pressed(KEY_BACKSPACE) && strlen(password) > 0) {
                display_putc(display, 0xdb, display->width/2-26/2+1+strlen(password), 21, 0x00);
                password[strlen(password)-1] = '\0';
                break;
            }
            if (is_key_pressed(KEY_TAB)) {
                display_putc(display, 0xdb, display->width/2-26/2+1+strlen(password), 21, 0x00);
                field = (field + 1) % 2;
                break;
            }
            if (isch(k) && strlen(password) < 20) {
                display_putc(display, '*', display->width/2-26/2+1+strlen(password), 21, 0x03);
                password[strlen(password)] = k;
            }
            display_putc(display, 0xdb, display->width/2-26/2+1+strlen(password), 21, 0x07);
            break;
    }
}


void jmenu_draw_register(struct Display* display)
{
    
}

void jmenu_update_register(struct Display* display)
{
    
}


void jmenu_draw_add_substance(struct Display* display)
{
    const char* title = \
"            _      _    _              "\
"           /_\\  __| |__| |             "\
"          / _ \\/ _` / _` |             "\
" ___     /_/ \\_\\__,_\\__,_|             "\
"/ __|_  _| |__ __| |_ __ _ _ _  __ ___ "\
"\\__ \\ || | '_ (_-<  _/ _` | ' \\/ _/ -_)"\
"|___/\\_,_|_.__/__/\\__\\__,_|_||_\\__\\___|";
    int y = 0;
    for (int i = 0; i < strlen(title); i++) {
        display_putc(display, title[i], 13+i%39, 2+y, 0x0c);
        if ((i+1)%39 == 0) y++;
    }

    draw_box_title(display, display->width/2-26/2, 15, 26, 2, 0x0c, 0x00, "Substance name");
    
    display_print(display, "Enter", display->width/2-26/2+2, 23, 0x08);
    display_print(display, "to add substance", display->width/2-26/2+8, 23, 0x07);
}

void jmenu_update_add_substance(struct Display* display)
{
    static char substance_name[16];
    int k = get_key_pressed();
    if (is_key_pressed(KEY_BACKSPACE) && strlen(substance_name) > 0) {
        display_putc(display, 0x00, display->width/2-26/2+1+strlen(substance_name), 16, 0x00);
        substance_name[strlen(substance_name)-1] = 0;
    }
    if (isch(k) && strlen(substance_name) < 16) {
        display_putc(display, k, display->width/2-26/2+1+strlen(substance_name), 16, 0x0b);
        substance_name[strlen(substance_name)] = k;
    }
    display_putc(display, 0xdb, display->width/2-26/2+1+strlen(substance_name), 16, 0x07);
    
    if (is_key_pressed(KEY_ENTER)) {
        _substances = realloc(_substances, sizeof(struct substance_t)*(_header->substance_count + 1));
        strcpy(_substances[_header->substance_count++].name, substance_name);
        jmenu_encrypt(password_hash);
        memset(substance_name, 0, 16);
        clear_box(display, 0, 0, display->width, display->height);
        
        jmenu_draw_dosages(display);
    }
    
}


void jmenu_draw_add_dosage(struct Display* display)
{
    
}

void jmenu_update_add_dosage(struct Display* display)
{
    
}


void jmenu_draw_offline(struct Display* display)
{
    jmenu_update = jmenu_update_offline;
    
    const char* title = \
    " _              _      " \
    "| |   ___  __ _(_)_ _  " \
    "| |__/ _ \\/ _` | | ' \\ " \
    "|____\\___/\\__, |_|_||_|" \
    "          |___/        ";
    int y = 0;
    for (int i = 0; i < strlen(title); i++) {
        display_putc(display, title[i], 20+i%23, 8+y, 0x0c);
        if ((i+1)%23 == 0) y++;
    }
    
    display_print(display, "F5", display->width/2-26/2-2, 25, 0x08);
    display_print(display, "to generate offline database", display->width/2-26/2+1, 25, 0x07);
    
    draw_box_title(display, display->width/2-26/2, 18, 26, 2, 0x0c, 0x00, "Password");
    
    display_print(display, "SubstanceJournal", 1, 46, 0x09);
    display_print(display, "by", 18, 46, 0x07);
    display_print(display, ".xnc", 21, 46, 0x08);
    display_print(display, "based on", 26, 46, 0x03);
    display_print(display, "djournal", 35, 46, 0x09);
    display_print(display, "by", 44, 46, 0x07);
    display_print(display, "boomer678", 47, 46, 0x08);
}

void jmenu_update_offline(struct Display* display)
{
    static bool generating = false;
    static char password[20];
    
    if (is_key_pressed(KEY_F5)) {
        memset(password, '\0', 20);
        clear_box(display, display->width/2-26/2, 18, 26, 2);
        draw_box_title(display, display->width/2-26/2, 18, 26, 2, 0x0c, 0x00, "New password");
        clear_box(display, 0, 25, display->width, 1);
        
        generating = true;
    }
    
    if (generating) {
        int k = get_key_pressed();
        if (is_key_pressed(KEY_BACKSPACE) && strlen(password) > 0) {
            display_putc(display, 0x00, display->width/2-26/2+1+strlen(password), 19, 0x00);
            password[strlen(password)-1] = 0;
        }
        if (isch(k) && strlen(password) < 20) {
            display_putc(display, '*', display->width/2-26/2+1+strlen(password), 19, 0x03);
            password[strlen(password)] = k;
        }
        display_putc(display, 0xdb, display->width/2-26/2+1+strlen(password), 19, 0x07);
        if (is_key_pressed(KEY_ENTER)) {
            AES_init_ctx_iv(&_ctx, password, "thisisnot16bytes");
            
            struct header_t* header = malloc(sizeof(struct header_t));
            struct entry_t* entry = malloc(sizeof(struct entry_t));
            struct substance_t* substance = malloc(sizeof(struct substance_t));
            
            strcpy(header->seq, "SJ01");
            header->entry_count = 0;
            header->substance_count = 0;    
            
            AES_CBC_encrypt_buffer(&_ctx, (uint8_t*)header, sizeof(*header));
            
            FILE* f = fopen("data.bin", "wb");
            fwrite(header, sizeof(struct header_t), 1, f);
            fwrite(entry, sizeof(struct entry_t), 1, f);
            fwrite(substance, sizeof(struct substance_t), 1, f);
            fclose(f);
            
            free(header);
            free(entry);
            free(substance);
            
            memset(password, '\0', 20);
            clear_box(display, display->width/2-26/2, 18, 26, 2);
            jmenu_draw_offline(display);
            generating = false;
        }
        return;
    }
    
    int k = get_key_pressed();
    if (is_key_pressed(KEY_BACKSPACE) && strlen(password) > 0) {
        display_putc(display, 0x00, display->width/2-26/2+1+strlen(password), 19, 0x00);
        password[strlen(password)-1] = 0;
    }
    if (isch(k) && strlen(password) < 20) {
        display_putc(display, '*', display->width/2-26/2+1+strlen(password), 19, 0x03);
        password[strlen(password)] = k;
    }
    display_putc(display, 0xdb, display->width/2-26/2+1+strlen(password), 19, 0x07);
    if (is_key_pressed(KEY_ENTER)) {
        AES_init_ctx_iv(&_ctx, password, "thisisnot16bytes");
        strcpy(password_hash, password);
        
        jmenu_decrypt(password);
        if (!_entries) {
            clear_box(display, display->width/2-26/2, 18, 26, 2);
            draw_box_title(display, display->width/2-26/2, 18, 26, 2, 0x0c, 0x00, "Password");
            display_print(display, "!ERROR!", display->width/2-26/2, 16, 0x08);
            memset(password, '\0', 20);
            return;
        }
        
        clear_box(display, 0, 0, display->width, display->height);
        memset(password, '\0', 20);
        
        jmenu_draw_dosages(display);
    }
}

jmenu_ptr jmenu_get_ptr()
{
    return jmenu_update;
}

void jmenu_free()
{
    if (_header) free(_header);
    if (_entries) free(_entries);
    if (_substances) free(_substances);
}
