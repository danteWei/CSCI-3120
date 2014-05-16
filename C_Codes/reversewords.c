#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STRLEN 80

int read_line(char line[], int len);
void reverse_words(char* line);

int main(void) {
  char line[STRLEN+1];

  printf("Enter a line of text:\n");
  read_line(line, STRLEN);

  reverse_words(line);

  puts(line);

  return 0;
}

int read_line(char line[], int len) {
  int ch, i = 0;

  while ((ch = getchar()) != '\n') 
    if (i < len) 
      line[i++] = ch;
  
  line[i] = '\0';

  return i;
}

void reverse_words(char* line) {
  int len;
  char* buffer;
  int read_pos, write_pos, word_start, word_end;

  len = strlen(line);
  buffer = malloc(__________);
  if (buffer == NULL) {
    printf("out of memory in reverse_words.\n");
    exit(EXIT_FAILURE);
  }

  read_pos = len - 1;
  write_pos = 0;

  while (read_pos >= 0) {
    if (line[read_pos] == ' ') {
      buffer[write_pos++] = line[read_pos--];
    }
    else {
      word_end = read_pos;

      while (read_pos >= 0 && line[read_pos] != ' ')
	read_pos--;

      word_start = read_pos + 1;

      while (word_start <= word_end)
	buffer[write_pos++] = line[_____________];
    }
  }

  buffer[write_pos] = '\0';
  strcpy(line, buffer);

  ____________________;
}
