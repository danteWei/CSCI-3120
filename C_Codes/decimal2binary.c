/* Program: decimal2binary.c */
#include <stdio.h>
#include <stdbool.h> /* C99 Standard */
#include <stdlib.h>

#define STACK_SIZE 100

typedef int Bit;

Bit contents[STACK_SIZE];
int top = 0;               /* index to next available spot */

void make_empty();
bool is_empty();
bool is_full();
void push(Bit i);
Bit pop();
void stack_overflow();
void stack_underflow();

int main() {
  int decimal;
  Bit bit;

  printf("Enter a decimal integer: ");
  scanf("%d", &decimal);

  while (decimal > 0) {
    bit = decimal % 2;
    __________;
    decimal /= 2;
  }
  
  printf("This number can be expressed in binary as: ");

  while (!is_empty())
    printf("%d", _______);

  printf("\n");

  return 0;
}

void make_empty() {
  top = 0;
}

bool is_empty() {
  return top == 0;
}

bool is_full() {
  return top == STACK_SIZE;
}

void push(Bit i) {
  if (is_full())
    stack_overflow();
  else
    contents[top++] = i;
}

Bit pop(void) {
  if (is_empty())
    stack_underflow();
  else
    return contents[_________];
}

void stack_overflow(void) {
  printf("Error: stack overflow!\n");
  exit(EXIT_FAILURE);
}

void stack_underflow(void) {
  printf("Error: stack underflow!\n");
  exit(EXIT_FAILURE);
}
