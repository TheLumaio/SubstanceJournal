#include "journal.h"

static const char* ivecstr = "NicotineIsntGood";

int journal_new(char* password)
{
    unsigned char keystr[AES_BLOCK_SIZE];
    memcpy(keystr, password, AES_BLOCK_SIZE);

    AES_KEY key;
    int a = AES_set_encrypt_key(keystr, 128, &key);

    header_t header;
    header.substance_count = 0;
    header.entry_count = 0;
    header.seq[0] = 'S';
    header.seq[1] = 'J';
    header.seq[2] = '0';
    header.seq[3] = '2';

    header_t enc_header;
    unsigned char ivec[AES_BLOCK_SIZE];
    memcpy(ivec , ivecstr, AES_BLOCK_SIZE);

    int num = 0;
    AES_cfb128_encrypt((const unsigned char*)&header, (unsigned char*)&enc_header, sizeof(header_t), &key, ivec, &num, AES_ENCRYPT);

    FILE* f = fopen("data.bin", "wb");
    if (!f) {
        fclose(f);
        return JOURNAL_FILE_ERROR;
    }

    fwrite(&enc_header, sizeof(header_t), 1, f);
    fclose(f);

    return JOURNAL_OK;
}

int journal_decrypt(char* password)
{
    _entries = list_create(8);
    _substances = list_create(8);

    unsigned char keystr[AES_BLOCK_SIZE];
    memcpy(keystr, password, AES_BLOCK_SIZE);

    AES_KEY key;
    int a = AES_set_encrypt_key(keystr, 128, &key);
    unsigned char ivec[AES_BLOCK_SIZE];
    memcpy(ivec , ivecstr, AES_BLOCK_SIZE);

    int num = 0;

    FILE* f = fopen("data.bin", "rb");
    if (!f) {
        fclose(f);
        return JOURNAL_FILE_ERROR;
    }

    void* enc_header = malloc(sizeof(header_t));
    fread(enc_header, sizeof(header_t), 1, f);

    header_t header;
    AES_cfb128_encrypt(enc_header, (unsigned char*)&header, sizeof(header_t), &key, ivec, &num, AES_DECRYPT);
    if (header.seq[0] == 'S' && header.seq[1] == 'J' && header.seq[2] == '0' && header.seq[3] == '2') {
        printf("File decrypted successfully\n");
    } else {
        printf("Error decrypting file\n");
        fclose(f);
        return JOURNAL_FILE_ERROR;
    }

    for (int i = 0; i < header.entry_count; i++)
    {
        void* enc_entry = malloc(sizeof(entry_t));
        fread(enc_entry, sizeof(entry_t), 1, f);

        entry_t* entry = malloc(sizeof(entry_t));
        memcpy(ivec , ivecstr, AES_BLOCK_SIZE);
        AES_cfb128_encrypt(enc_entry, (unsigned char*)&entry, sizeof(entry_t), &key, ivec, &num, AES_DECRYPT);

        list_append(_entries, entry);
    }
    
    for (int i = 0; i < header.substance_count; i++)
    {
        void* enc_substance = malloc(sizeof(substance_t));
        fread(enc_substance, sizeof(substance_t), 1, f);

        substance_t* substance = malloc(sizeof(substance_t));
        memcpy(ivec , ivecstr, AES_BLOCK_SIZE);
        AES_cfb128_encrypt(enc_substance, (unsigned char*)&substance, sizeof(substance_t), &key, ivec, &num, AES_DECRYPT);

        list_append(_substances, substance);
    }

    fclose(f);
    return JOURNAL_OK;
}

int journal_encrypt(char* password)
{
    
}

void journal_add_substance(substance_t* sub)
{
    list_append(_substances, sub);
}

list_t* journal_get_entries() { return _entries; }
list_t* journal_get_substances() { return _substances; }
char** journal_get_scales() {
    if (!_scales) {
        _scales = list_create(4);
        list_append(_scales, "UG");
        list_append(_scales, "MG");
        list_append(_scales, "G");
        list_append(_scales, "ML");
    }
    return (char**)_scales->data;
}
