#ifndef JOURNAL_H
#define JOURNAL_H

#include <stdio.h>
#include <openssl/aes.h>
#include "display/display.h"
#include "display/display_tools.h"
#include "list.h"

typedef struct header_t {
    char seq[4];
    uint16_t entry_count;
    uint16_t substance_count;
} header_t;

typedef struct entry_t {
    uint16_t substance;
    uint16_t amount;
    uint8_t day;
    uint8_t month;
    uint8_t year; // 2000+year
    // TODO: Interactions
} entry_t;

typedef struct substance_t {
    uint16_t uid;
    uint8_t scale;
    char name[17];
} substance_t;

enum journal_status {
    JOURNAL_OK,
    JOURNAL_FILE_ERROR
};

static list_t* _scales;
static list_t* _entries;
static list_t* _substances;

int journal_new(char*);
int journal_decrypt(char*);
int journal_encrypt(char*);

void journal_add_substance(substance_t*);

list_t* journal_get_entries();
list_t* journal_get_substances();
char** journal_get_scales();

#endif
