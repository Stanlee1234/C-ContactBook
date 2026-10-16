#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGTH 100
#define MAX_CONTACTS 100
/* Four quoted fields, doubled quotes, delimiters, CRLF and NUL. */
#define CSV_LINE_LENGTH (INPUT_LENGTH * 8 + 16)

char *FILENAME = "contact_book_data.csv";

typedef struct Contact {
  char name[INPUT_LENGTH];
  char address[INPUT_LENGTH];
  char email[INPUT_LENGTH];
  char number[INPUT_LENGTH];
} Contact;

Contact contact_array[MAX_CONTACTS];

void trim_newline(char *str);
int add_new_entry();
int read_from_csv(Contact contact_array[]);
void print_contact(const Contact *contact);
int list_contacts();

int main(int argc, char *argv[]) {
  if (argc > 1) {
    FILENAME = argv[1];
  }

  char choice_buffer[INPUT_LENGTH];
  while (1) {
    printf("--- Contact Book Menu ---\n");
    printf("A: Add new contact\n");
    printf("L: List all contacts\n");
    printf("Q: Quit\n");
    printf("Enter choice: ");

    if (fgets(choice_buffer, INPUT_LENGTH, stdin) == NULL)
      break;
    trim_newline(choice_buffer);

    if (strlen(choice_buffer) == 0)
      continue;

    char choice = tolower(choice_buffer[0]);

    if (choice == 'a') {
      add_new_entry();
    } else if (choice == 'l') {
      list_contacts();
    } else if (choice == 'q') {
      printf("Goodbye!\n");
      return 0;
    } else {
      printf("\n'%c' is not a known command.\n\n", choice_buffer[0]);
    }
  }

  return 0;
}

void trim_newline(char *str) {
  size_t len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
}

int add_new_entry() {
  FILE *fptr;

  fptr = fopen(FILENAME, "a");
  if (fptr == NULL) {
    printf("Could not open %s for writing\n", FILENAME);
    return 1;
  }

  char name[INPUT_LENGTH];
  char address[INPUT_LENGTH];
  char email[INPUT_LENGTH];
  char number[INPUT_LENGTH];

  printf("Enter Name: ");
  if (fgets(name, INPUT_LENGTH, stdin) == NULL) {
    fclose(fptr);
    return 1;
  }
  trim_newline(name);

  printf("Enter Address: ");
  if (fgets(address, INPUT_LENGTH, stdin) == NULL) {
    fclose(fptr);
    return 1;
  }
  trim_newline(address);

  printf("Enter Email: ");
  if (fgets(email, INPUT_LENGTH, stdin) == NULL) {
    fclose(fptr);
    return 1;
  }
  trim_newline(email);

  printf("Enter Number: ");
  if (fgets(number, INPUT_LENGTH, stdin) == NULL) {
    fclose(fptr);
    return 1;
  }
  trim_newline(number);

  const char *fields[] = {name, address, email, number};
  for (int i = 0; i < 4; i++) {
    if (i > 0)
      fputc(',', fptr);
    fputc('"', fptr);
    for (const char *c = fields[i]; *c; c++) {
      if (*c == '"')
        fputc('"', fptr);
      fputc(*c, fptr);
    }
    fputc('"', fptr);
  }
  fputc('\n', fptr);
  fclose(fptr);

  printf("\nSaved contact!\n\n");

  return 0;
}

/* Contacts are one physical line: the input prompts do not accept newlines.
 * Accept quoted or legacy unquoted fields; reject malformed/oversized rows
 * rather than silently shifting or truncating a contact.
 */
static int parse_contact_csv(const char *line, Contact *contact) {
  Contact parsed = {0};
  char *fields[] = {parsed.name, parsed.address, parsed.email, parsed.number};
  const char *p = line;
  for (int field = 0; field < 4; field++) {
    size_t length = 0;
    int quoted = (*p == '"');
    if (quoted)
      p++;
    while (*p) {
      if (quoted) {
        if (*p == '"') {
          p++;
          if (*p != '"') {
            quoted = 0;
            break;
          }
        } else if (*p == '\r' || *p == '\n') {
          return 0;
        }
      } else if (*p == ',' || *p == '\r' || *p == '\n') {
        break;
      } else if (*p == '"') {
        return 0;
      }
      if (length >= INPUT_LENGTH - 1)
        return 0;
      fields[field][length++] = *p++;
    }
    if (quoted)
      return 0;
    if (field < 3) {
      if (*p++ != ',')
        return 0;
    } else {
      if (*p == '\r')
        p++;
      if (*p == '\n')
        p++;
      if (*p != '\0')
        return 0;
    }
  }
  *contact = parsed;
  return 1;
}

int read_from_csv(Contact contact_array[]) {
  FILE *fptr = fopen(FILENAME, "r");
  char buffer[CSV_LINE_LENGTH];
  int count = 0;
  if (fptr == NULL)
    return 0;

  while (count < MAX_CONTACTS && fgets(buffer, sizeof(buffer), fptr)) {
    if (strchr(buffer, '\n') == NULL && !feof(fptr)) {
      int c;
      while ((c = fgetc(fptr)) != '\n' && c != EOF) {}
      continue;
    }
    if (parse_contact_csv(buffer, &contact_array[count]))
      count++;
  }
  fclose(fptr);
  return count;
}

void print_contact(const Contact *contact) {
  printf("Name: %s\n", contact->name);
  printf("Address: %s\n", contact->address);
  printf("Email: %s\n", contact->email);
  printf("Number: %s\n", contact->number);
  printf("\n");
}

int list_contacts() {
  int contact_count = read_from_csv(contact_array);

  if (contact_count == 0) {
    printf("\nNo contacts found.\n\n");
    return 0;
  }

  printf("\n--- Contact List ---\n");
  for (int i = 0; i < contact_count; i++) {
    print_contact(&contact_array[i]);
  }

  return 0;
}
