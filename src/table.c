#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "log.h"
#include "table.h"

table *table_create() {
	table *new_table = malloc(sizeof(table));
	if (new_table == NULL) {
		LOG_ERROR("Failed to allocate memory for table");
		return NULL;
	}

	new_table->entries = malloc(DEFAULT_TABLE_SIZE * sizeof(table_entry));
	if (new_table->entries == NULL) {
		LOG_ERROR("Failed to allocate memory for table entries");
		return NULL;
	}

	new_table->size = 0;
	new_table->capacity = DEFAULT_TABLE_SIZE;

	return new_table;
}

void table_destroy(table *table) {
	// Not freeing the keys because this table implementation is used solely for headers,
	// so the char pointers for the keys will come from the previously allocated request 
	// buffer, which will be freed separately
	
	free(table->entries);
	free(table);
}

bool table_expand(table *table) {
	unsigned long new_capacity = table->capacity * 2;
	if (new_capacity > UINT32_MAX) {
		LOG_ERROR("Maximum table size reached, table could not be expanded.");
		return false;
	}

	table_entry *new_entry_list = malloc(new_capacity * sizeof(table_entry));
	if (new_entry_list == NULL) {
		LOG_ERROR("Failed to allocate more memory for hash table entries");
		return false;
	}

	for (uint32_t i = 0; i < table->capacity; i++) {
		if (table->entries[i].key != NULL) {
			entries_insert(new_entry_list, table->entries[i].key, table->entries[i].value, new_capacity);
		}
	}

	free(table->entries);
	table->capacity = new_capacity;
	table->entries = new_entry_list;

	return true;
}

table_value table_get(table *table, const char *key) {
	uint32_t index = hash_key(key) % table->capacity;
	while (strcmp(key, table->entries[index].key) != 0) {
		if (index >= table->capacity) {
			index = 0;
		} else {
			index++;
		}
	}

	return table->entries[index].value;
}

bool table_insert(table *table, const char *key, table_value value) {
	if (table->size >= (3 * table->capacity / 4)) { // Expand
		if (!table_expand(table)) {
			LOG_ERROR("Failed to expand table");
			return false;
		}
	}

	entries_insert(table->entries, key, value, table->capacity);
	return true;
}

void entries_insert(table_entry *table_entries, const char *key, table_value value, uint32_t capacity) {
	uint32_t index = hash_key(key) % capacity;
	while (table_entries[index].key != NULL) {
		if (index >= capacity) {
			index = 0;
		} else {
			index++;
		}
	}

	table_entries[index].key = key;
	table_entries[index].value = value;
}

uint32_t hash_key(const char *key) {
	uint32_t hash = FNV_OFFSET_BASIS;
	size_t byte = 0;

	while (key[byte]) {
		hash = hash * FNV_PRIME;
		hash ^= key[byte];
		byte++;
	}

	return hash;
}

void table_print(table *table) {
	printf("{\n");

	uint32_t index = 0;
	while (index < table->capacity) {
		if (table->entries[index].key != NULL) {
			const char *key = table->entries[index].key;
			table_value entry = table->entries[index].value;
			switch (entry.kind) {
				case UINT:
					printf("\t%s: %d\n", key, entry.data.uint_value);
					break;
				case STRING:
					printf("\t%s: ", key);
					fflush(stdout);
					write(STDOUT_FILENO, entry.data.string_value.string, entry.data.string_value.string_len);
					printf("\n");
					break;
			}
		}
		index++;
	}

	printf("}\n");
}
