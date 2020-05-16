#ifndef JOURNAL_H
#define JOURNAL_H

#define CBC 1

#include <stdio.h>
#include "aes.h"
#include "display/display.h"
#include "display/display_tools.h"

typedef void(*jmenu_ptr)();

struct header_t {
    unsigned char _pad[7];
    char seq[5];
    uint16_t entry_count;
    uint16_t substance_count;
}; // 16

struct entry_t {
    unsigned char _pad[8];
    uint16_t substance_index;
    uint16_t dose;
    char scale[16];
    uint8_t day;
    uint8_t month;
    uint16_t year;
}; // 32

struct substance_t {
    unsigned char _pad[14];
    uint16_t uid;
    char name[16];
}; // 32

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

void jmenu_draw_dosages();
void jmenu_update_dosages();

void jmenu_draw_login();
void jmenu_update_login();

void jmenu_draw_register();
void jmenu_update_register();

void jmenu_draw_add_substance();
void jmenu_update_add_substance();

void jmenu_draw_add_dosage();
void jmenu_update_add_dosage();

void jmenu_draw_offline();
void jmenu_update_offline();

jmenu_ptr jmenu_get_ptr();
void jmenu_free();

#endif
