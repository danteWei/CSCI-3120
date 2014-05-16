#include <stdio.h>
#include <math.h>

#define LEN 10

void statistics(double array[], int len, double* min, double* max, double* average, double* stddev);

int main() {
  double array[LEN] = {27.8, 31.235, 18.9, 32.55, 20.3,
                       36.0, 49.1, 29.54, 30.7, 19.5};
  double min, max, average, stddev;

  statistics(array, LEN, ____________________________________);

  printf("minimum: %.3f; maximum: %.3f; average: %.3f; standard deviation: %.3f\n", min, max, average, stddev);

  return 0;
}

void statistics(double array[], int len, double* min, double* max, double* average, double* stddev) {
  int i;

  _________ = array[0];
  _________ = array[0];
  _________ = array[0];

  for (i = 1; i < LEN; i++) {
    if (array[i] < *min) 
      *min = array[i];
    else if (array[i] > *max)
      *max = array[i];

    *average += array[i];
  }

  *average /= len;

  *stddev = 0;
  for (i = 0; i < LEN; i++) {
    *stddev += (array[i] - *average) * (array[i] - *average);
  }

  *stddev = sqrt(*stddev / len);
}
