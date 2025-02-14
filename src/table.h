#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_TABLE_SIZE 8
#define FNV_PRIME 16777619UL
#define FNV_OFFSET_BASIS 2166136261UL

typedef enum {
    STRING,
    UINT
} table_value_type;

typedef struct {
    table_value_type kind; 
    union data {
        struct {
            char *string;
            uint32_t string_len;
        } string_value;
        uint32_t uint_value;
    } data;
} table_value;

typedef struct {
    const char *key; 
    table_value value;
} table_entry;

typedef struct {
    table_entry *entries;
    uint32_t size;
    uint32_t capacity;
} table;

table *table_create();
void table_destroy(table *table);
bool table_expand(table *table);
table_value table_get(table *table, const char *key);
bool table_insert(table *table, const char *key, table_value value);
void entries_insert(table_entry *table_entries, const char *key, table_value value, uint32_t capacity);
uint32_t hash_key(const char *key);
void table_print(table *table);

#endif // !TABLE_H
