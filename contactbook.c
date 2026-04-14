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

// Function signatures
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

  fptr = fopen(FILENAME, "a"); // 'a' means open in append mode
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
  fclose(fptr); // close file when done

  printf("\nSaved contact!\n\n");

  return 0; // 0 = success
}

int read_from_csv(Contact contact_array[]) {
  FILE *fptr;
  char buffer[(INPUT_LENGTH * 4 + 5)]; // calc the max size of a line
  int i = 0;

  fptr = fopen(FILENAME, "r"); // open file in read mode
  if (fptr == NULL) {
    return 0; // Return 0 contacts if file doesn't exist
  }

  while (fgets(buffer, sizeof(buffer), fptr)) {
    char *p = buffer;
    // Skip empty lines or whitespace-only lines
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
      p++;
    if (*p == '\0')
      continue;

    char *new_line_pointer = strchr(buffer, '\n'); // remove the new line char
    if (new_line_pointer)
      *new_line_pointer = '\0';

    char *field = strtok(buffer, ",");
    int x = 0; // also using this as an iterator to know where to place the data
               // in the array

    while (field != NULL) {
      if (x == 0) { // if name
        strncpy(contact_array[i].name, field, INPUT_LENGTH - 1);
        contact_array[i].name[INPUT_LENGTH - 1] = '\0';
      } else if (x == 1) { // if address
        strncpy(contact_array[i].address, field, INPUT_LENGTH - 1);
        contact_array[i].address[INPUT_LENGTH - 1] = '\0';
      } else if (x == 2) { // if email
        strncpy(contact_array[i].email, field, INPUT_LENGTH - 1);
        contact_array[i].email[INPUT_LENGTH - 1] = '\0';
      } else if (x == 3) { // if number
        strncpy(contact_array[i].number, field, INPUT_LENGTH - 1);
        contact_array[i].number[INPUT_LENGTH - 1] = '\0';
      }
      x++;
      field = strtok(
          NULL,
          ","); // NULL tells it to continue where it left off on last string
    }

    i++;
    if (i >= MAX_CONTACTS)
      break;
  }

  fclose(fptr);

  return i; // number of contacts loaded
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
  for (int i = 0; i < contact_count; i++) { // once per contact
    print_contact(&contact_array[i]);       // & means the memory location of
  } // so its a pointer to that element of the array

  return 0; // 0 = success
}
