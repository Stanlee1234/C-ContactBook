#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGTH 100
#define MAX_CONTACTS 100

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
  fgets(name, INPUT_LENGTH, stdin);
  trim_newline(name);

  printf("Enter Address: ");
  fgets(address, INPUT_LENGTH, stdin);
  trim_newline(address);

  printf("Enter Email: ");
  fgets(email, INPUT_LENGTH, stdin);
  trim_newline(email);

  printf("Enter Number: ");
  fgets(number, INPUT_LENGTH, stdin);
  trim_newline(number);

  fprintf(fptr, "%s,%s,%s,%s\n", name, address, email, number);
  fclose(fptr);

  printf("\nSaved contact!\n\n");

  return 0;
}

int read_from_csv(Contact contact_array[]) {
  FILE *fptr;
  char buffer[(INPUT_LENGTH * 4 + 5)];
  int i = 0;

  fptr = fopen(FILENAME, "r");
  if (fptr == NULL) {
    return 0;
  }

  while (fgets(buffer, sizeof(buffer), fptr)) {
    char *p = buffer;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
      p++;
    if (*p == '\0')
      continue;

    char *new_line_pointer = strchr(buffer, '\n');
    if (new_line_pointer)
      *new_line_pointer = '\0';

    char *field = strtok(buffer, ",");
    int x = 0;

    while (field != NULL) {
      if (x == 0) {
        strncpy(contact_array[i].name, field, INPUT_LENGTH - 1);
        contact_array[i].name[INPUT_LENGTH - 1] = '\0';
      } else if (x == 1) {
        strncpy(contact_array[i].address, field, INPUT_LENGTH - 1);
        contact_array[i].address[INPUT_LENGTH - 1] = '\0';
      } else if (x == 2) {
        strncpy(contact_array[i].email, field, INPUT_LENGTH - 1);
        contact_array[i].email[INPUT_LENGTH - 1] = '\0';
      } else if (x == 3) {
        strncpy(contact_array[i].number, field, INPUT_LENGTH - 1);
        contact_array[i].number[INPUT_LENGTH - 1] = '\0';
      }
      x++;
      field = strtok(NULL, ",");
    }

    i++;
    if (i >= MAX_CONTACTS)
      break;
  }

  fclose(fptr);

  return i;
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
