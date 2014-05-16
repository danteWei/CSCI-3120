#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STRLEN 80

int read_line(char line[], int len);
void reverse_words(char* line);
void reverse_string(char* str, int start, int end);

int main(void) {
  char line[STRLEN+1];

  printf("Enter a line of text:\n");
  read_line(line, STRLEN);

  reverse_words(line);

  puts(line);

  return 0;
}

void reverse_string(char* str, int start, int end) {
  char temp;

  while (start < end) {
    temp = str[start];
    str[start] = str[end];
    str[end] = temp;

    start++;
    end--;
  }
}

void reverse_words(char* line) {
  int start = 0, end=0, len;

  len = strlen(line);

  reverse_string(line, 0, len-1);

  while (end < len) {
    if (line[end] != ' ') {
      start = end;

      while (end < len && line[end] != ' ')
	end++;

      end--;

      reverse_string(line, start, end);
    }

    end++;
  }
}

int read_line(char line[], int len) {
  int ch, i = 0;

  while ((ch = getchar()) != '\n') 
    if (i < len) 
      line[i++] = ch;
  
  line[i] = '\0';

  return i;
}
