#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M 1000
#define N 1000


int main() {

double err, tol;
int iter, iter_max;
int m = M;
int n = N;
double A[n][m];
double Anew[n][m];

iter_max = 1000;
iter = 0;
tol = 0.0001;
err = 1.0;


for( int i = 1; i < m; i++) {

 A[0][i] = 100;

 A[1][i] = 100;

 for( int j = 2; j < n; j++) A[j][i] = 10;

}

// main loop from NVIDIA slides

while ( err > tol && iter < iter_max ) {

 err = 0.0;

#pragma omp parallel
 {
 #pragma omp for reduction(max:err)
 for( int j = 1; j < n-1; j++) {
  for( int i = 1; i < m-1; i++) {

   Anew[j][i] = 0.25 * (A[j][i+1] + A[j][i-1] + A[j-1][i] + A[j+1][i]);

   err = fmax( err, fabs(Anew[j][i] - A[j][i]) );

  }
 }

 #pragma omp for
 for( int j = 1; j < n-1; j++) {
  for( int i = 1; i < m-1; i++) {

   A[j][i] = Anew[j][i];

  }
 }

}

 if ( iter++ % 100 == 0 || err <= tol )  printf("%5d, %0.6f\n", iter, err);

}
// main loop ends

return 0
}