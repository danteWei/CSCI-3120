/* Program: mergesort.c */
#include <stdio.h>

void mergesort(int array[], int lower, int upper);
void merge(int array[], int lower, int middle, int upper);

int main() {
  int len, i;

  printf("Enter the length of the array: ");
  scanf("%d", &len);

  int array[len];

  printf("Enter %d integers:\n", len);
  for (i = 0; i < len; i++) 
    scanf("%d", &array[i]);

  mergesort(array, 0, len-1);

  printf("The sorted array is: \n");
  for (i = 0; i < len; i++) {
    printf("%d", array[i]);
    if (i < len - 1) printf(" ");
  }
  printf("\n");

  return 0;
}

void mergesort(int array[], int lower, int upper) {
  if (lower < upper) {
    int middle = (lower + upper) / 2; 
    mergesort(array, lower, middle);
    mergesort(array, middle+1, upper);
    merge(array, lower, middle, upper);
  }
}

/* middle element is a part of lower sub-array */
void merge(int array[], int lower, int middle, int upper) {
  int len_left = middle - lower + 1;
  int len_right = upper - middle;
  int left[len_left], right[len_right];
  int i, j, k;

  for (i = 0, j=lower; i < len_left; i++, j++) 
    left[i] = array[j];

  for (i = 0, j = _____________ ; i < len_right; i++, j++)
    right[i] = array[j];

  i = j = 0; k = lower;
  while (i < len_left && j < len_right) {
    if (left[i] <= right[j]) array[k++] = left[ _______ ];
    else                     array[k++] = right _______ ];
  }

  while (i < len_left)  array[k++] = left [ _______ ];
  while (j < len_right) array[k++] = right[ _______ ];
}
