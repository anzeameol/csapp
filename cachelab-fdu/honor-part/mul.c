/*
 * mul.c - Matrix multiply C = A * B
 *
 * Each multiply function must have a prototype of the form:
 * void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);
 *
 * A multiply function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);

/*
 * multiply_submit - This is the solution multiply function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "multiply submission", as the driver
 *     searches for that string to identify the multiply function to
 *     be graded.
 */
// 10e0e0
// 14e0e0
char mul_submit_desc[] = "multiply submission";
void mul_submit(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int a[8], b[8];
    for (int ci = 0; ci < 32; ci += 8)
    {
        for (int cj = 0; cj < 32; cj += 8)
        {
            for (int ck = 0; ck < 32; ck += 8)
            {
                for (int k = ci; k < ci + 8; k++)
                {
                    for (int m = ck; m < ck + 8; m++)
                    {
                        for (int n = 0; n < 8; n++)
                        {
                            a[n] = A[k][cj + n];
                        }
                        b[m - ck] = 0;
                        for (int n = 0; n < 8; n++)
                        {
                            b[m - ck] += a[n] * B[cj + n][m];
                        }
                    }
                    if (cj == 0)
                    {
                        for (int m = ck; m < ck + 8; m++)
                        {
                            C[k][m] = b[m - ck];
                        }
                    }
                    else
                    {
                        for (int m = ck; m < ck + 8; m++)
                        {
                            C[k][m] += b[m - ck];
                        }
                    }
                }
            }
        }
    }
}

/*
 * mul - A simple multiply function, not optimized for the cache.
 */
char mul_desc[] = "multiply submission";
void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k, tmp;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            tmp = 0;
            for (k = 0; k < M; k++)
            {
                tmp += A[i][k] * B[k][j];
            }
            C[i][j] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your multiply
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     multiply strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerMulFunction(mul_submit, mul_submit_desc);

    /* Register any additional multiply functions */
    // registerMulFunction(mul, mul_desc);
}

/*
 * is_multiply - This helper function checks if C is the multiply of
 *     A and B. You can check the correctness of your multiply by calling
 *     it before returning from the multiply function.
 */
int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k;
    int num = 0;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            num = 0;
            for (k = 0; k < M; k++)
            {
                num += A[i][k] * B[k][j];
            }
            if (num != C[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}
