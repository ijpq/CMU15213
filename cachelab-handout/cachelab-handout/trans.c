/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

#define MAGICSIZE 8

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void Optimize(int M, int N, int A[M][N], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	Optimize(M,N,A,B);
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char OptimizeM32_desc[] = "Optimize for M32N32";
void OptimizeM32(int M, int N, int A[N][M], int B[M][N]) {

	int iLocal, jLocal, iMov, jMov;
	int tmp;
	for (iMov=0; iMov<N;iMov+=8 ) {
		for (jMov=0;jMov<M;jMov+=8) {
			for (iLocal=0;iLocal<MAGICSIZE;iLocal++) {
				for (jLocal=0;jLocal<MAGICSIZE;jLocal++) {
					tmp = A[iLocal+iMov][jLocal+jMov];
					B[jLocal+jMov][iLocal+iMov] = tmp;
					
				}
			}
		}
	}
	
}

char OptimizeM32_rowlocal_desc[] = "Optimize for 12 local vars";
void OptimizeM32_rowlocal(int M, int N, int A[N][M], int B[M][N]) {
	
	int iLocal, jLocal, iMov, jMov;
	int tmp8[8];
	for (iMov=0; iMov<N;iMov+=8 ) {
		for (jMov=0;jMov<M;jMov+=8) {
			for (iLocal=0;iLocal<MAGICSIZE;iLocal++) {
				for (jLocal=0;jLocal<MAGICSIZE;jLocal++) {
					tmp8[jLocal] = A[iLocal+iMov][jLocal+jMov];	
				}
				for (jLocal=0;jLocal<MAGICSIZE;jLocal++) {
					B[jLocal+jMov][iLocal+iMov] = tmp8[jLocal];
				}
				
			}
		}
	}
}


void OptimizeM64(int M, int N, int A[N][M], int B[M][N]) {
	//
	
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 

	registerTransFunction(OptimizeM32, OptimizeM32_desc);
	registerTransFunction(OptimizeM32_rowlocal, OptimizeM32_rowlocal_desc);
	registerTransFunction();

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

