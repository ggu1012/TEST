/* Kim Seonghoon, y2016142212 */

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
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    /* For submission */

    // iteration variable
    int i, j, k;

    // Save values in register
    int ra, rb, rc, rd, re, rx, ry, rz;

    if (M == 32 && N == 32) {
        // stride of main block = 8
        // vertical block movement. down
        for (i = 0; i < N; i += 8) {
            // horizontal block movement. left
            for (j = M - 8; j >= 0; j -= 8) {
                /* inside one block */
                // vertical stride of sub-block = 8
                // vertical movement
                for (k = i; k < i + 8; k++) {
                    // First, load A elements
                    ra = A[k][j];
                    rb = A[k][j + 1];
                    rc = A[k][j + 2];
                    rd = A[k][j + 3];
                    re = A[k][j + 4];
                    rx = A[k][j + 5];
                    ry = A[k][j + 6];
                    rz = A[k][j + 7];
                    // Then, store B elements
                    B[j][k] = ra;
                    B[j + 1][k] = rb;
                    B[j + 2][k] = rc;
                    B[j + 3][k] = rd;
                    B[j + 4][k] = re;
                    B[j + 5][k] = rx;
                    B[j + 6][k] = ry;
                    B[j + 7][k] = rz;
                }
            }
        }
    }

    else if (M == 64 && N == 64) {  // 8x8 main block
        // 4x8 sub block
        // 4x4 4x4 sub block

        // Main block 8x8
        // vertical movement
        for (i = 0; i < N; i += 8) {
            // horizontal movement
            for (j = 0; j < M; j += 8) {
                // vertical movement
                // Sub block 4x8
                // vertical movement
                for (k = i; k < i + 4; ++k) {
                    // saving values in horizontal movement
                    ra = A[k][j];
                    rb = A[k][j + 1];
                    rc = A[k][j + 2];
                    rd = A[k][j + 3];
                    re = A[k][j + 4];
                    rx = A[k][j + 5];
                    ry = A[k][j + 6];
                    rz = A[k][j + 7];

                    // Save half part of B
                    // to reduce eviction
                    // saving in vertical movement
                    B[j][k] = ra;
                    B[j + 1][k] = rb;
                    B[j + 2][k] = rc;
                    B[j + 3][k] = rd;

                    // Other half is temporarily saved
                    // at nearby B block, where "hit" is induced
                    B[j][k + 4] = re;
                    B[j + 1][k + 4] = rx;
                    B[j + 2][k + 4] = ry;
                    B[j + 3][k + 4] = rz;
                }

                // Sub block 4x4 (i+4, j) ~ (i+7, j+3)
                for (k = j; k < j + 4; ++k) {
                    ra = A[i + 4][k];
                    rb = A[i + 5][k];
                    rc = A[i + 6][k];
                    rd = A[i + 7][k];

                    // now, load temporarily saved block and get out
                    // to replace with original block at that space
                    re = B[k][i + 4];
                    rx = B[k][i + 5];
                    ry = B[k][i + 6];
                    rz = B[k][i + 7];

                    // Then replace the block with new value
                    // a, b, c, d to get advantage of remaining "hit" condition
                    // induced by e, x, y ,z load
                    B[k][i + 4] = ra;
                    B[k][i + 5] = rb;
                    B[k][i + 6] = rc;
                    B[k][i + 7] = rd;

                    // Align B array with the data
                    // from 8x8 block
                    // B[j+4][i~i+4] ~ B[j+7][i~i+4]
                    B[k + 4][i] = re;
                    B[k + 4][i + 1] = rx;
                    B[k + 4][i + 2] = ry;
                    B[k + 4][i + 3] = rz;
                }

                // Handle another remaining 4x4 sub block
                for (k = i + 4; k < i + 8; ++k) {
                    ra = A[k][j + 4];
                    rb = A[k][j + 5];
                    rc = A[k][j + 6];
                    rd = A[k][j + 7];

                    B[j + 4][k] = ra;
                    B[j + 5][k] = rb;
                    B[j + 6][k] = rc;
                    B[j + 7][k] = rd;
                }
            }
        }
    }

    else if (M == 61 && N == 67) {
        // stride =8
        // vertical
         for (i = 0; i < N; i += 8) {
             //horizontal
            for (j = 0; j < M; j += 8) {
                // block inside
                for (k = i; k < i + 8 && k < N; ++k) {
                    ra = A[k][j];
                    rb = A[k][j + 1];
                    rc = A[k][j + 2];
                    rd = A[k][j + 3];
                    re = A[k][j + 4];
                    // if block reaches the end, skip this part
                    if (j + 4 != M - 1) {
                        rx = A[k][j + 5];
                        ry = A[k][j + 6];
                        rz = A[k][j + 7];
                    }
                    // Then, store B elements
                    B[j][k] = ra;
                    B[j + 1][k] = rb;
                    B[j + 2][k] = rc;
                    B[j + 3][k] = rd;
                    B[j + 4][k] = re;
                    // if block reaches the end, skip this part
                    if (j + 4 != M - 1) {
                        B[j + 5][k] = rx;
                        B[j + 6][k] = ry;
                        B[j + 7][k] = rz;
                    }
                }
            }
        }
    }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the
 * cache.
 */

char tmp_desc[] = "61x67";
void __61x67(int M, int N, int A[N][M], int B[M][N]) {
    /* For submission */

    int i, j, k;
    int a, b, c, d, e, x, y, z;

    if (M == 32 && N == 32) {
        // stride of main block = 8
        // vertical block movement. down
        for (i = 0; i < N; i += 8) {
            // horizontal block movement. left
            for (j = M - 8; j >= 0; j -= 8) {
                /* inside one block */
                // vertical stride of sub-block = 4
                // vertical movement
                for (k = i; k < i + 8; k++) {
                    // First, load A elements
                    a = A[k][j];
                    b = A[k][j + 1];
                    c = A[k][j + 2];
                    d = A[k][j + 3];
                    e = A[k][j + 4];
                    x = A[k][j + 5];
                    y = A[k][j + 6];
                    z = A[k][j + 7];
                    // Then, store B elements
                    B[j][k] = a;
                    B[j + 1][k] = b;
                    B[j + 2][k] = c;
                    B[j + 3][k] = d;
                    B[j + 4][k] = e;
                    B[j + 5][k] = x;
                    B[j + 6][k] = y;
                    B[j + 7][k] = z;
                }
            }
        }
    }

    else if (M == 64 && N == 64) {
        // 8x8 main block
        // 4x8 sub block
        // 4x4 4x4 sub block

        // Main block 8x8
        // vertical movement
        for (i = 0; i < N; i += 8) {
            // horizontal movement
            for (j = 0; j < M; j += 8) {
                // vertical movement
                // Sub block 4x8
                // vertical movement
                for (k = i; k < i + 4; ++k) {
                    // saving values in horizontal movement
                    a = A[k][j];
                    b = A[k][j + 1];
                    c = A[k][j + 2];
                    d = A[k][j + 3];
                    e = A[k][j + 4];
                    x = A[k][j + 5];
                    y = A[k][j + 6];
                    z = A[k][j + 7];

                    // Save half part of B
                    // to reduce eviction
                    // saving in vertical movement
                    B[j][k] = a;
                    B[j + 1][k] = b;
                    B[j + 2][k] = c;
                    B[j + 3][k] = d;

                    // Other half is temporarily saved
                    // at nearby B block, where "hit" is induced
                    B[j][k + 4] = e;
                    B[j + 1][k + 4] = x;
                    B[j + 2][k + 4] = y;
                    B[j + 3][k + 4] = z;
                }

                // Sub block 4x4 (i+4, j) ~ (i+7, j+3)
                for (k = j; k < j + 4; ++k) {
                    a = A[i + 4][k];
                    b = A[i + 5][k];
                    c = A[i + 6][k];
                    d = A[i + 7][k];

                    // now, load temporarily saved block and get out
                    // to replace with original block at that space
                    e = B[k][i + 4];
                    x = B[k][i + 5];
                    y = B[k][i + 6];
                    z = B[k][i + 7];

                    // Then replace the block with new value
                    // a, b, c, d to get advantage of remaining "hit" condition
                    // induced by e, x, y ,z load
                    B[k][i + 4] = a;
                    B[k][i + 5] = b;
                    B[k][i + 6] = c;
                    B[k][i + 7] = d;

                    // Align B array with the data
                    // from 8x8 block
                    // B[j+4][i~i+4] ~ B[j+7][i~i+4]
                    B[k + 4][i] = e;
                    B[k + 4][i + 1] = x;
                    B[k + 4][i + 2] = y;
                    B[k + 4][i + 3] = z;
                }

                // Handle another remaining 4x4 sub block
                for (k = i + 4; k < i + 8; ++k) {
                    a = A[k][j + 4];
                    b = A[k][j + 5];
                    c = A[k][j + 6];
                    d = A[k][j + 7];

                    B[j + 4][k] = a;
                    B[j + 5][k] = b;
                    B[j + 6][k] = c;
                    B[j + 7][k] = d;
                }
            }
        }
    }

    else if (M == 61 && N == 67) {
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                // block inside
                for (k = i; k < i + 8 && k < N; ++k) {
                    a = A[k][j];
                    b = A[k][j + 1];
                    c = A[k][j + 2];
                    d = A[k][j + 3];
                    e = A[k][j + 4];
                    // if block reaches the end, skip this part
                    if (j + 4 != M - 1) {
                        x = A[k][j + 5];
                        y = A[k][j + 6];
                        z = A[k][j + 7];
                    }
                    // Then, store B elements
                    B[j][k] = a;
                    B[j + 1][k] = b;
                    B[j + 2][k] = c;
                    B[j + 3][k] = d;
                    B[j + 4][k] = e;
                    // if block reaches the end, skip this part
                    if (j + 4 != M - 1) {
                        B[j + 5][k] = x;
                        B[j + 6][k] = y;
                        B[j + 7][k] = z;
                    }
                }
            }
        }
    }
}

char first_desc[] = "First Prototype. block-split";
void zig_zag(int M, int N, int A[N][M], int B[M][N]) {
    int st_row = 8;
    int st_col = 8;  // stride for row/column
    int tmp;         // save element

    // Every explanation is based on array A.
    // vertical block movement
    for (int i = N - 1; i >= 0; i -= st_col) {
        // horizontal block movement
        for (int j = M - 1; j >= 0; j -= st_row) {
            /* inside one block */
            // vertical movement
            for (int k = i; k > i - st_col; --k) {
                // horizontal movement
                for (int l = j; l > j - st_row; --l) {
                    tmp = A[k][l];
                    B[l][k] = tmp;
                }
            }
        }
    }
}

char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
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
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(__61x67, tmp_desc);
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
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
