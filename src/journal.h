#ifndef JOURNAL_H
#define JOURNAL_H

#include <stdio.h>
#include <openssl/aes.h>
#include "xnc/display/display.h"
#include "xnc/display/display_tools.h"
#include "xnc/list.h"

typedef struct header_t {
    char seq[4];
    uint16_t entry_count;
    uint16_t substance_count;
} header_t;

typedef struct entry_t {
    uint8_t substance;
    uint16_t amount;
    uint8_t day;
    uint8_t month;
    uint8_t year; // 2000+year
    uint8_t hour;
    uint8_t minute;
    char note[33];
    // TODO: Notes
} entry_t;

typedef struct substance_t {
    char name[17];
    uint16_t uid;
    uint8_t scale;
    // TODO: Interactions
} substance_t;

enum journal_status {
    JOURNAL_OK,
    JOURNAL_FILE_ERROR
};

static list_t* _scales;
static list_t* _entries;
static list_t* _substances;
static char* _passwd;

int journal_new(char*);
int journal_decrypt(char*);
int journal_encrypt();

void journal_add_substance(substance_t*);
void journal_add_entry(entry_t*);

list_t* journal_get_entries_at(int, int, int);

list_t* journal_get_entries();
list_t* journal_get_substances();
char** journal_get_scales();

#endif
