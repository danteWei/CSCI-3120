#include <stdio.h>
#include <stdlib.h>

#define NAME_LEN 25

struct node {
  int number;
  char name[NAME_LEN+1];
  struct node* next;
};


struct node* insert(struct node* student_list);
void print_student(struct node* student);
void print_list(struct node* student_list);
void search(struct node* student_list);
struct node* delete(struct node* student_list);
void delete_list(struct node* student_list);

int read_line(char line[], int len);

int main(void) {
  int option;
  struct node* student_list = NULL;

  for (;;) {
    printf("\n");
    printf("1: Add a student\n");
    printf("2: Search for a student by number\n");
    printf("3: Delete a student by number\n");
    printf("4: Display all students\n");
    printf("0: Exit\n");
    printf("\n");

    printf("Enter an option: ");
    if ((scanf ("%d", &option) != 1)) {
      printf("Failure to read an option\n");
      exit(EXIT_FAILURE);
    }


    while (getchar() != '\n');

    if(option == 0)
      break;

    switch(option) {
    case 1:
      student_list = insert(student_list);
      break;

    case 2:
      search(student_list);
      break;

    case 3:
      student_list = delete(student_list);
      break;

    case 4:
      print_list(student_list);
      break;

    default:
      printf("Incorrect option\n");
    }
  }

  delete_list(student_list);

  return 0;
}

struct node* insert(struct node* student_list) {
  struct node* student = malloc(sizeof(struct node));

  if (student == NULL) {
    printf("Out of memory.\n");
    exit(EXIT_FAILURE);
  }

  printf("Enter student's name: ");
  read_line(student->name, NAME_LEN);
  printf("Enter student's number: ");
  if (scanf("%d", &student->number) != 1) {
    printf("Incorrect student number.\n");
    exit(EXIT_FAILURE);
  }

  student->next = _______________;

  return student;
}

void print_student(struct node* student) {
  printf("Number: %d;  Name: %s\n", student->number, student->name);
}

void print_list(struct node* student_list) {
  while (student_list != NULL) {
    print_student(student_list);
    student_list = student_list->next;
  }
}

void search(struct node* student_list) {
  int number;

  printf("Enter student number: ");
  if (scanf("%d", &number) != 1) {
    printf("Incorrect student number.\n");
    exit(EXIT_FAILURE);
  }

  while (student_list != NULL && number != student_list->number) 
    student_list = _______________________;

  if (student_list == NULL)
    printf("Not found.\n");
  else
    print_student(student_list);
}

struct node* delete(struct node* student_list) {
  int number;
  struct node *prev, *cur;

  printf("Enter student number: ");
  if (scanf("%d", &number) != 1) {
    printf("Incorrect student number.\n");
    exit(EXIT_FAILURE);
  }

  for (cur = student_list, prev = NULL;
       cur != NULL && cur -> number != number;
       prev = cur, cur = cur->next)
    ;

  if (cur == NULL)
    return student_list;

  if (prev == NULL)
    student_list = student_list->next;
  else
    prev->next = _____________;

  free(cur);
  return student_list;
}

int read_line(char line[], int len) {
  int ch, i = 0;

  while ((ch = getchar()) != '\n') 
    if (i < len) 
      line[i++] = ch;
  
  line[i] = '\0';

  return i;
}

void delete_list(struct node* student_list) {
  struct node* temp;

  while (student_list != NULL) {
    temp = student_list;
    student_list = student_list->next;
    free(__________);
  }
}
