/* File:     
 *     main.c 
 *
 * Purpose:  
 *     Computes a shared-memory parallel program for matrix multiplication 
 *	   using pthreads
 *
 * Input:
 *     n: order of matrix
 *     A, B the matrices to be multiplied
 *
 * Output:
 *     C: the product matrix
 *
 * Compile:  gcc -g -Wall -o main main.c -lpthread -lm
 * Usage:
 *     main <thread_count>
 *
 * Notes:  
 *     1.  Local storage for A, B, C is dynamically allocated.
 *     2.  Number of threads (thread_count) should evenly divide both 
 *         n.  The program doesn't check for this.
 *     3.  We use a 2-dimensional array for A and B and compute subscripts
 *         using the formulae P[x][y] where x = floor(k/p) and y = k%sqrt(p)
 *     4.  Distribution of A, B, and C is logical:  all three are 
 *         globally shared.
 *
 * IPP:    Section 4.3 (pp. 159 and ff.).  Also Section 4.10 (pp. 191 and 
 *         ff.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "lab1_IO.h"
#include "timer.h"

/* Global variables */
int thread_count;
int n;
int **A; int **B; int **C;
double start; double end; double Time;

//double* A;
//double* B;
//double* C;

/* Parallel function */
void *pth_mat_mult(void* rank);

int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;

   Lab1_loadinput(&A, &B, &n);

   C = malloc(n*sizeof(int *));

   int i;

   for (i = 0; i < n; i++){
      C[i] = malloc(n * sizeof(int));
   }

   thread_count = atoi(argv[1]);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
	  pthread_create(&thread_handles[thread], NULL,
		 pth_mat_mult, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
	  pthread_join(thread_handles[thread], NULL);
   GET_TIME(end);

   Time = end - start;

   Lab1_saveoutput(C, &n, Time);

   free(A);
   free(B);
	free(C);

   return 0;
}  /* main */



/*------------------------------------------------------------------
 * Function:       pth_mat_mult
 * Purpose:        Multiply an nxn matrix A by an nxn matrix B
 * In arg:         rank
 * Global in vars: A, B, n, thread_count
 * Global out var: C
 */
void *pth_mat_mult(void* rank) {

   long my_rank = (long) rank;
   int i;
   int x, y;

   x = floor(my_rank/sqrt(thread_count));
   y = (int)((int)my_rank % (int)sqrt(thread_count));

   // int local_n = n/sqrt(thread_count); 
   //int my_first_row = my_rank*local_n;
   //int my_last_row = (my_rank+1)*local_n - 1;

	C[x][y] = 0;

   for (i = 0; i < n; i++) {
   	  //printf("(%d)(%d) + ", A[x*n+i], B[i*n+y]);
	   C[x][y] += A[x][i]*B[i][y];
   }

   //printf("Value at P_%d%d is %d\n", x, y, C[x*n+y]);
   return NULL;
}  /* Pth_mat_vect */














