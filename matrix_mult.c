// Sumber 1: https://www.w3schools.in/c-program/matrix-multiplication-in-c/
// Sumber 2: https://www.techiedelight.com/find-execution-time-c-program/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"

int main(void)
{
  int c, d, p, q, m, n, k, tot = 0;
  int fst[10][10], sec[10][10], mul[10][10];

  printf(" Please insert the number of rows and columns for first matrix \n ");
  scanf("%d%d", &m, &n);

//   printf(" Insert your matrix elements : \n ");
//   for (c = 0; c < m; c++)
//     for (d = 0; d < n; d++)
//       scanf("%d", &fst[c][d]);
// TODO: isi random int
 
  printf(" Please insert the number of rows and columns for second matrix\n");
  scanf(" %d %d", &p, &q);

  if (n != p)
    printf(" Your given matrices cannot be multiplied with each other. \n ");
  else
  {
    printf(" Insert your elements for second matrix \n ");
 
    // for (c = 0; c < p; c++)
    //   for (d = 0; d < q; d++)
    //     scanf("%d", &sec[c][d] );
    // TODO: isi random int juga untuk 2nd matrix

    // Scatter Gather + MPI init dll mulai dari sini harusnya

    double time = 0.0;
    clock_t begin = clock();

    for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++) {
        for (k = 0; k < p; k++) {
          tot = tot + fst[c][k] * sec[k][d];
        }
        mul[c][d] = tot;
        tot = 0;
      }
    }

    // End di ini
    
    clock_t end = clock();
 
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    time += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds", time);

    // printf(" The result of matrix multiplication or product of the matrices is: \n "); 
    // for (c = 0; c < m; c++) {
    //   for (d = 0; d < q; d++)
    //     printf("%d \t", mul[c][d] );
    //   printf(" \n ");
    // }
    // Ini jangan di print, kalo n nya 1000+ terminalnya jebol
  }
  return 0;
}
