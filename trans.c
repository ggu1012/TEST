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

    int i, j, k;
    int a, b, c, d, e, x, y, z, w;

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
        /* Main block */
        // stride of main block = 8
        // horizontal block movement.
        for (j = 0; j < M; j += 8) {
            // vertical block movement. down
            for (i = 0; i < N; i += 8) {
                /* sub-block */
                // if the main block is in diagonal position,
                // index is all the same.
                if (i == j) {
                    // stride of sub-block = 4
                    // horizontal movement
                    for (a = j; a < j + 8; a += 4) {
                        // vertical movement
                        for (b = i; b < i + 8; b += 4) {
                            /* inside sub block */
                            for (c = b; c < b + 4; c++) {
                                // First, load A elements
                                x = A[c][a];
                                y = A[c][a + 1];
                                z = A[c][a + 2];
                                w = A[c][a + 3];

                                // Then, store B elements
                                B[a][c] = x;
                                B[a + 1][c] = y;
                                B[a + 2][c] = z;
                                B[a + 3][c] = w;
                            }
                        }
                    }

                } else {
                    /* sub-block */
                    // vertical stride of sub-block = 4
                    // vertical movement
                    for (a = j; a < j + 8; a += 4) {
                        // horizontal movement
                        for (b = i; b < i + 8; b += 4) {
                            /* inside sub block */
                            for (c = b; c < b + 4; c++) {
                                // First, load A elements
                                x = A[c][a];
                                y = A[c][a + 1];
                                z = A[c][a + 2];
                                w = A[c][a + 3];

                                // Then, store B elements
                                B[a][c] = x;
                                B[a + 1][c] = y;
                                B[a + 2][c] = z;
                                B[a + 3][c] = w;
                            }
                        }
                    }
                }
            }
        }
    }

    else if (M == 61 && N == 67) {
        // stride = 16
        for (i = 0; i < N; i += 16) {
            // horizontal block movement. left
            for (j = 0; j < M; j += 8) {
                // inside one block
                // Check smaller than N for N%8!=0 boundary
                for (k = i; k < (i + 16) && (k < N); ++k) {
                    for (x = j; (x < j + 16) && (j < M); ++x) {
                        if (x != k) {
                            B[x][k] = A[k][x];
                        } else {
                            a = x;  // save where diagonal
                            b = A[x][x];
                        }
                        B[a][a] = b;
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

char tmp_desc[] = "64x64";
void __64x64(int M, int N, int A[N][M], int B[M][N]) {
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
                    B[k+4][i] = e;
                    B[k+4][i+1] = x;
                    B[k+4][i+2] = y;
                    B[k+4][i+3] = z;
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
        // stride = 16
        for (i = 0; i < N; i += 16) {
            // horizontal block movement. left
            for (j = 0; j < M; j += 16) {
                // inside one block
                // Check smaller than N for N%8!=0 boundary
                for (k = i; k < (i + 16) && (k < N); ++k) {
                    for (x = j; (x < j + 16) && (j < M); ++x) {
                        if (x != k) {
                            B[x][k] = A[k][x];
                        } else {
                            a = x;  // save where diagonal
                            b = A[x][x];
                        }
                        B[a][a] = b;
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

char tmp[] = "temporary";
void nn(int M, int N, int A[N][M], int B[M][N]) { /* For submission */

    int i, j, k;
    int a, b, c, d, e, f, g, h;  // save element

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
                    f = A[k][j + 5];
                    g = A[k][j + 6];
                    h = A[k][j + 7];
                    // Then, store B elements
                    B[j][k] = a;
                    B[j + 1][k] = b;
                    B[j + 2][k] = c;
                    B[j + 3][k] = d;
                    B[j + 4][k] = e;
                    B[j + 5][k] = f;
                    B[j + 6][k] = g;
                    B[j + 7][k] = h;
                }
            }
        }
    }

    else if (M == 64 && N == 64) {
    }

    else if (M == 67 && N == 61) {
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
    registerTransFunction(__64x64, tmp_desc);
    registerTransFunction(zig_zag, first_desc);
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
