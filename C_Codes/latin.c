#include <stdio.h>

int main() {
  int size, i, j;

  printf("Enter the size of the latin square: ");
  scanf("%d", &size);
  if (size < 1) return 1;

  int square[size][size], occurs[size+1];

  printf("Enter a matrix (%d by %d) of integers in the range [1-%d]: \n",
	 size, size, size);
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++) {
      scanf("%d", _______________ );
      
      if (square[i][j] > size || square[i][j] <= 0) {
	printf("Error: element out of range.\n");
	return 1;
      }
    }

  for (i = 0; i < size; i++) {
    for (j = 1; j <= size; j++)
      occurs[j] = 0;

    for (j = 0; j < size; j++) {
      if ( _________________ > 0) {
	printf("This is not a Latin square.\n");
	return 1;
      }
      else
	occurs[ ______________ ] = 1;
    }
  }

  for (j = 0; j < size; j++) {
    for (i = 1; i <= size; i++)
      occurs[i] = 0;

    for (i = 0; i < size; i++) {
      if (_____________________ > 0) {
	printf("This is not a Latin square.\n");
	return 1;
      }
      else 
	occurs[ _____________ ] = 1;
    }
  }

  printf("This is a Latin square.\n");
  return 0;
}
