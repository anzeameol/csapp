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

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */

#define A48
#define A96
#define BS1 12
#define BS2 12
#define BS3 12

/*
[A48,B48] ,[A96,B96,C96,D96,E96] are different models
BS1, BS2, BS3 are the block_size for each given N
here are the performance (rated according to the number of misses):
// <model-block_size> : <misses>

// 48 (500-800)
// A-12: 398
// A-6: 634
// B-12: 432
// B-6: 664

// 96 (2100-3000)
// A: 1788
// B: 2100
// C: 2388
// D-12: 9988
// D-6: 2564
// E-6: 2444

// 93-99 (3000-4000)
// 12: 2728
// 8: 3176
// 6: 3207
// 4: 4172
*/

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (N == 48)
    {
#ifdef A48
        int a[12];
        for (int ci = 0; ci < N; ci += BS1)
        {
            for (int cj = 0; cj < N; cj += BS1)
            {
                for (int k = 0; k < BS1; k++)
                {
                    for (int m = 0; m < BS1; m++)
                    {
                        a[m] = A[ci + k][cj + m];
                    }
                    for (int m = 0; m < BS1; m++)
                    {
                        B[cj + k][ci + m] = a[m];
                    }
                }
                for (int k = 0; k < BS1; k++)
                {
                    for (int m = k; m < BS1; m++)
                    {
                        a[0] = B[cj + k][ci + m];
                        B[cj + k][ci + m] = B[cj + m][ci + k];
                        B[cj + m][ci + k] = a[0];
                    }
                }
            }
        }
#endif
#ifdef B48
        int a[12];
        for (int ci = 0; ci < N; ci += BS1)
        {
            for (int cj = 0; cj < N; cj += BS1)
            {
                for (int k = ci; k < ci + BS1; k++)
                {
                    for (int m = 0; m < BS1; m++)
                    {
                        a[m] = A[k][cj + m];
                    }
                    for (int m = 0; m < BS1; m++)
                    {
                        B[cj + m][k] = a[m];
                    }
                }
            }
        }
#endif
    }
    else if (N == 96)
    {
#ifdef A96
        int a[12];
        for (int ci = 0; ci < N; ci += BS2)
        {
            for (int cj = 0; cj < N; cj += BS2)
            {
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        a[m] = A[ci + k][cj + m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m][ci + k] = a[m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m][ci + k + BS2 / 2] = a[m + BS2 / 2];
                    }
                }
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        if (m < BS2 / 2)
                        {
                            a[m] = A[ci + m + BS2 / 2][cj + k];
                        }
                        else
                        {
                            a[m] = B[cj + k][ci + m];
                        }
                    }
                    for (int m = 0; m < BS2; m++)
                    {
                        if (m < BS2 / 2)
                        {
                            B[cj + k][ci + m + BS2 / 2] = a[m];
                        }
                        else
                        {
                            B[cj + k + BS2 / 2][ci + m - BS2 / 2] = a[m];
                        }
                    }
                }
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        a[m] = A[ci + k + BS2 / 2][cj + m + BS2 / 2];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m + BS2 / 2][ci + k + BS2 / 2] = a[m];
                    }
                }
            }
        }
#endif
#ifdef B96
        int a[12];
        for (int ci = 0; ci < N; ci += BS2)
        {
            for (int cj = 0; cj < N; cj += BS2)
            {
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        a[m] = A[ci + k][cj + m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m][ci + k] = a[m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m][ci + k + BS2 / 2] = a[m + BS2 / 2];
                    }
                }
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        a[m] = B[cj + k][ci + m + BS2 / 2];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + k + BS2 / 2][ci + m] = a[m];
                    }
                }
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        a[m] = A[ci + k + BS2 / 2][cj + m + BS2 / 2];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m + BS2 / 2][ci + k + BS2 / 2] = a[m];
                    }
                }
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        a[m] = A[ci + k + BS2 / 2][cj + m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m][ci + k + BS2 / 2] = a[m];
                    }
                }
            }
        }
#endif
#ifdef C96
        int a[12];
        for (int ci = 0; ci < N; ci += BS2)
        {
            for (int cj = 0; cj < N; cj += BS2)
            {
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        a[m] = A[ci + k][cj + m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m][ci + k] = a[m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m][ci + k + BS2 / 2] = a[m + BS2 / 2];
                    }
                }
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        a[m] = A[ci + k + BS2 / 2][cj + m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m + BS2 / 2][ci + k] = a[m];
                    }
                    for (int m = 0; m < BS2 / 2; m++)
                    {
                        B[cj + m + BS2 / 2][ci + k + BS2 / 2] = a[m + BS2 / 2];
                    }
                }
                for (int k = 0; k < BS2 / 2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        if (m < BS2 / 2)
                        {
                            a[m] = B[cj + k + BS2 / 2][ci + m];
                        }
                        else
                        {
                            a[m] = B[cj + k][ci + m];
                        }
                    }
                    for (int m = 0; m < BS2; m++)
                    {
                        if (m < BS2 / 2)
                        {
                            B[cj + k][ci + m + BS2 / 2] = a[m];
                        }
                        else
                        {
                            B[cj + k + BS2 / 2][ci + m - BS2 / 2] = a[m];
                        }
                    }
                }
            }
        }
#endif
#ifdef D96
        int a[12];
        for (int ci = 0; ci < N; ci += BS2)
        {
            for (int cj = 0; cj < N; cj += BS2)
            {
                for (int k = ci; k < ci + BS2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        a[m] = A[k][cj + m];
                    }
                    for (int m = 0; m < BS2; m++)
                    {
                        B[cj + m][k] = a[m];
                    }
                }
            }
        }
#endif
#ifdef E96
        int a[12];
        for (int ci = 0; ci < N; ci += BS2)
        {
            for (int cj = 0; cj < N; cj += BS2)
            {
                for (int k = 0; k < BS2; k++)
                {
                    for (int m = 0; m < BS2; m++)
                    {
                        a[m] = A[ci + k][cj + m];
                    }
                    for (int m = 0; m < BS2; m++)
                    {
                        B[cj + k][ci + m] = a[m];
                    }
                }
                for (int k = 0; k < BS2; k++)
                {
                    for (int m = k; m < BS2; m++)
                    {
                        a[0] = B[cj + k][ci + m];
                        B[cj + k][ci + m] = B[cj + m][ci + k];
                        B[cj + m][ci + k] = a[0];
                    }
                }
            }
        }
#endif
    }
    else
    {
        int ci;
        int cj;
        int a[12];
        for (cj = 0; cj < M / BS3 * BS3; cj += BS3)
        {
            for (ci = 0; ci < N / BS3 * BS3; ci += BS3)
            {
                for (int k = 0; k < BS3; k++)
                {
                    for (int m = 0; m < BS3; m++)
                    {
                        a[m] = A[ci + k][cj + m];
                    }
                    for (int m = 0; m < BS3; m++)
                    {
                        B[cj + m][ci + k] = a[m];
                    }
                }
            }
        }
        for (int i = 0; i < N; i++)
        {
            for (int j = cj; j < M; j++)
            {
                B[j][i] = A[i][j];
            }
        }
        for (int i = ci; i < N; i++)
        {
            for (int j = 0; j < cj; j++)
            {
                B[j][i] = A[i][j];
            }
        }
    }
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

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
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
    // registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
