#include "table.h"

int main(int argc, char *argv[])
{
	table *header_table = table_create();
	table_value t;
	t.kind = UINT;
	t.data.uint_value = 32;
	table_insert(header_table, "Hello", t);
	t.kind = STRING;
	t.data.string_value.string_len = 5;
	t.data.string_value.string = "hello";
	table_insert(header_table, "Greeting", t);
	table_print(header_table);
	return 0;
}
