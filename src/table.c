#include <cstdint>
#include <stdlib.h>

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

bool *table_expand(table *table) {
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
			entries_insert(new_entry_list, table->entries[i].key, *table->entries[i].value, new_capacity);
		}
	}

	table->capacity = new_capacity;

	return true;
}

table_value table_get(table *table, const char *key) {
	if (table->size >= (3 / 4) * table->capacity) { // Expand

	}
}

bool table_insert(table *table, const char *key, table_value value) {

}

bool entries_insert(table_entry *table_entries, const char *key, table_value value, uint32_t capacity) {

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



