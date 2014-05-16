#include <stdio.h>

void mergesort(int* array, int lower, int upper);
void merge(int* array, int lower, int middle, int upper);

int main(void) {
  int len;
  int *p;

  printf("Enter the length of the array: ");
  scanf("%d", &len);

  int array[len];

  printf("Enter %d integers:\n", len);

  for (p = array; p < array+len; p++) 
    scanf("%d", ________);

  mergesort(array, 0, len-1);

  printf("The sorted array is: \n");
  for (p = array; p < array+len; p++) {
    printf("%d", _________);
    if (p < array + len - 1)
      printf(" ");
  }
  printf("\n");

  return 0;
}


void mergesort(int* array, int lower, int upper) {
  if (lower < upper) {
    int middle = (lower + upper) / 2; 
    mergesort(array, lower, middle);
    mergesort(array, middle+1, upper);
    merge(array, lower, middle, upper);
  }
}

void merge(int* array, int lower, int middle, int upper) {
  int len_left = middle - lower + 1;
  int len_right = upper - middle;
  int left[len_left], right[len_right];
  int *p, *q, *r;

  for (p = left, r = array + lower; p < left + len_left; p++, r++) 
    *p = *r;

  for (q = right; q < right + len_right; q++, r++)
    *q = *r;

  p = left;
  q = right;
  r = array + lower;

  while (p < left + len_left && q < right + len_right) {
    if (*p <= *q)
      *r++ = *p++;
    else 
      *r++ = _________;
  }

  while (p < left + len_left) 
    *r++ = *p++;

  while (q < _________________) 
    *r++ = *q++;
}

