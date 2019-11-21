#ifndef JOURNAL_H
#define JOURNAL_H

#define CBC 1

#include <stdio.h>
#include "aes.h"
#include "display.h"
#include "display_tools.h"

typedef void(*jmenu_ptr)(struct Display*);

struct header_t {
    unsigned char _pad[7];
    char seq[5];
    uint16_t entry_count;
    uint16_t substance_count;
}; // 16

struct entry_t {
    unsigned char _pad[12];
    uint16_t substance_index;
    uint16_t dose;
    char scale[16];
    uint8_t day;
    uint8_t month;
    uint16_t year;
}; // 64

struct substance_t {
    char name[16];
}; // 16

static const char _days[7][9] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

static struct AES_ctx _ctx;
static struct header_t* _header = NULL;
static struct entry_t* _entries = NULL;
static struct substance_t* _substances = NULL;

static char password_hash[20];

static jmenu_ptr jmenu_update = NULL;

void jmenu_init();

static void jmenu_encrypt(char*);
static void jmenu_decrypt(char*);

void jmenu_draw_dosages(struct Display* display);
void jmenu_update_dosages(struct Display* display);

void jmenu_draw_login(struct Display* display);
void jmenu_update_login(struct Display* display);

void jmenu_draw_register(struct Display* display);
void jmenu_update_register(struct Display* display);

void jmenu_draw_add_substance(struct Display* display);
void jmenu_update_add_substance(struct Display* display);

void jmenu_draw_add_dosage(struct Display* display);
void jmenu_update_add_dosage(struct Display* display);

void jmenu_draw_offline(struct Display* display);
void jmenu_update_offline(struct Display* display);

jmenu_ptr jmenu_get_ptr();
void jmenu_free();

#endif
