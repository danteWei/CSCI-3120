#include <stdio.h>

#define LEN 10

int main(void) {
  int array[LEN];
  int lower, upper, middle, key;
  int i;

  printf("Enter %d numbers in ascending order:\n", LEN);

  for (i = 0; i < LEN; i++)
    scanf("%d", ________);

  lower = 0;
  upper = LEN - 1;
  middle = (lower + upper) / 2;

  printf("Enter the number to be searched for: ");
  scanf("%d", &key);

  while (array[middle] != key && lower <= upper) {
    if (key < array[middle])
      upper = ___________;
    else
      lower = ___________;

    middle = (lower + upper) / 2;
  }

  if (_____________)
    printf("%d is the %d-th number you entered.\n", key, middle+1);
  else
    printf("Not found.\n");

  return 0;
}
