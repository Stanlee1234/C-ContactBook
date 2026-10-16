#define main contactbook_main
#include "../contactbook.c"
#undef main
#include <assert.h>

int main(void) {
  Contact c = {0};
  assert(parse_contact_csv("Jane Doe,123 Main St,jane@example.com,555-0100\n", &c));
  assert(strcmp(c.number, "555-0100") == 0);
  assert(parse_contact_csv("\"Jane Doe\",\"123 Main St, Apt 4\",\"jane@example.com\",\"555-0100\"\n", &c));
  assert(strcmp(c.address, "123 Main St, Apt 4") == 0);
  assert(strcmp(c.email, "jane@example.com") == 0);
  assert(parse_contact_csv("John Doe,,john@example.com,555-0200\n", &c));
  assert(strcmp(c.address, "") == 0);
  assert(strcmp(c.email, "john@example.com") == 0);
  assert(parse_contact_csv("\"Jane \"\"JJ\"\" Doe\",\"\",\"\",\"\"\r\n", &c));
  assert(strcmp(c.name, "Jane \"JJ\" Doe") == 0);
  assert(strcmp(c.number, "") == 0);
  assert(parse_contact_csv(",,,", &c));
  assert(!parse_contact_csv("a,b,c,d,e\n", &c));
  assert(!parse_contact_csv("a,b,c\n", &c));
  assert(!parse_contact_csv("\"a,b,c,d\n", &c));
  assert(!parse_contact_csv("\"a\"junk,b,c,d\n", &c));
  assert(!parse_contact_csv("a\"b,c,d,e\n", &c));
  char row[CSV_LINE_LENGTH];
  memset(row, 'x', 100);
  strcpy(row + 100, ",b,c,d\n");
  assert(!parse_contact_csv(row, &c));
  puts("csv parser: pass");
  return 0;
}
