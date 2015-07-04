#include <stdio.h>
#include <stdlib.h>

#define GROW_FACTOR 2
#define RECIPROCAL_SHRINK_FACTOR 2
#define RECIPROCAL_MIN_LOAD 4

struct vector {
  int* array;
  int capacity;
  int size;
};

struct vector* create(int capacity);
int* access(struct vector* vec, int i);
void destroy(struct vector* vec);
void push_back(struct vector* vec, int value);
void pop_back(struct vector* vec);


int main(void) {
  int i;
  struct vector* vec;

  vec = create(5);

  for (i = 0; i < 25; i++)
    push_back(vec, i);

  for (i = 0; i < 25; i++)
    (*(access(vec, i)))++;

  printf("The current capactiy of the array is %d\n", vec->capacity);
  for (i = vec->size-1; i >= 0; i--)
    printf("%d ", *(access(vec, i)));
  printf("\n");

  for (i = 0; i < 21; i++)
    pop_back(vec);

  printf("The current capactiy of the array is %d\n", vec->capacity);
  for (i = vec->size-1; i >= 0; i--)
    printf("%d ", *(access(vec, i)));
  printf("\n");

  destroy(vec);

  return 0;
}

struct vector* create(int capacity) {
  struct vector* vec = malloc(sizeof(struct vector));
  if (vec == NULL) {
    printf("Out of memory.\n");
    exit(EXIT_FAILURE);
  }

  vec->capacity = capacity;

  vec->array = malloc(sizeof(int) * capacity);
  if (vec->array == NULL) {
    printf("Out of memory.\n");
    exit(EXIT_FAILURE);
  }
  vec->size = 0;

  return vec;
}

int* access(struct vector* vec, int i) {
  if (i < vec->size)
    return &(vec->array[i]);
  else {
    printf("Array out of bounds.\n");
    exit(EXIT_FAILURE);
  }
}

void destroy(struct vector* vec) {
  free(vec->array);
  free(vec);
}

void push_back(struct vector* vec, int value) {
  if (vec->size == vec->capacity) {
    int *temp;
    int i;

    temp = vec->array;

    vec->array = malloc((vec->capacity * GROW_FACTOR) * sizeof(int));
    if (vec->array == NULL) {
      printf("Out of memory.\n");
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < vec->size; i++)
      vec->array[i] = temp[i];

    vec->capacity *= GROW_FACTOR;

    free(temp);
  }

  vec->array[vec->size] = value;
  vec->size++;
}

void pop_back(struct vector* vec) {
  if (vec->size == 0) {
    printf("Attempting to remove the last element of an empty array.\n");
    exit(EXIT_FAILURE);
  }

  vec->size--;

  if(vec->size < vec->capacity / RECIPROCAL_MIN_LOAD) {

    int *temp;
    int i;

    temp = vec->array;

    vec->array = malloc((vec->capacity / RECIPROCAL_SHRINK_FACTOR) * sizeof(int));
    if (vec->array == NULL) {
      printf("Out of memory.\n");
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < vec->size; i++)
      vec->array[i] = temp[i];

    vec->capacity /= RECIPROCAL_SHRINK_FACTOR;

    free(temp);
  }
}
