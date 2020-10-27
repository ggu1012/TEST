/* Kim Seonghoon, y2016142212 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cachelab.h"

typedef struct {
    char op;                 // L, S or M
    unsigned long int addr;  // Address. uint64
    int size;
} trace_line;  // struct for trace line.

typedef struct {
    int s;       // 2^s is the number of sets
    int E;       // Associativity. # of lines per set
    int b;       // 2^b is the block size
} cache_struct;  // Cache structure

int main(int argc, char* argv[]) {
    int hit = 0;
    int miss = 0;
    int evict = 0;

    /* Address Structure variable */
    int index;
    int tag;
    int block_offset;

    FILE* trace = fopen(argv[8], "r");  // open trace file
    if (trace == NULL)                  // print error if file is invalid
        fprintf(stderr, "Invaild Trace File.\n");

    trace_line t;
    cache_struct cs;

    /* Extract cache structure from arguments */
    cs.s = atoi(argv[2]);
    cs.E = atoi(argv[4]);
    cs.b = atoi(argv[6]);

    /* Cache Line implementation */
    typedef struct {
        int index;
        int tag[cs.E];     // Stored tags
        int RUinfo[cs.E];  // Which tag is Recently Used? !! 0: Recently Used
    } cache_line;

    /* Cache Initialization w/ Memory allocation */
    cache_line* cache;
    cache = malloc(sizeof(cache_line) *
                   cs.s);  // initializes the total cache = cache_line * set

    int result = 0;  // Checks if file read reached end or not.

    while (1) {
        result = fscanf(trace, " %s %lx, %d", &t.op, &t.addr, &t.size);

        // END of trace file. break.
        if (result == EOF) break;

        // abandon instruction trace
        if (t.op == 'I') continue;

        /*
                            *** Address Implementation ***
        |        (64-s-b) bits       |      s bits     |   b bits   |
        |           tag              |      index      |  b_offset  |

        */

        /* Extract Cache Information from address*/
        index = t.addr << (64 - cs.s - cs.b) >> (64 - cs.s);
        tag = t.addr >> (cs.s + cs.b);
        block_offset = t.addr << (64 - cs.b) >> (64 - cs.b);

        /////////////////////////////////////////////////////
        /////// Important part. Cache Logics below //////////
        /////////////////////////////////////////////////////

        /* Temporary hit, miss variables for checking. 0 or 1 Values */
        int tmp_hit = 0;
        int tmp_miss = 0;
        int tmp_eviction = 0;

        /* First, checkout the memory access type */
        switch (t.op) {
            /* Load. */
            case 'L':
                /* Cache Walk. Checks if the index already exists in cache */
                for (int tmp_set = 0; tmp_set < cs.s; ++tmp_set) {
                    /* Index Exists! */
                    if (index == cache[tmp_set].index) {
                        /* Now Checks the tag exists in the cache line */
                        for (int tmp_line; tmp_line < cs.E; ++tmp_line) {
                            /* Hit ! */
                            if (tag == cache[tmp_set].tag[tmp_line]) {
                                tmp_hit = 1;
                                break;
                            } else {
                                /* Miss.. Tag Replacement with LRU line */
                                tmp_miss = 1;
                                /* Checks where cache.RUinfo == cs.E - 1 */                                
                                if (cache[tmp_set].RUinfo[tmp_line] == cs.E - 1) {
                                    /* tag update ! */
                                    cache[tmp_set].tag[tmp_line] = tag;
                                }
                            }
                        }
                    }
                }

            /* Store. */
            case 'S':

            /* Modify. Load and Store in order */
            case 'M':
        }

        printf("%c %lx, %d\n", t.op, t.addr, t.size);
        printf("Cache Info. tag = %d, index = %d, block_offset = %d\n", tag,
               index, block_offset);
    }

    printf("Cache structure. s=%d, E=%d, b=%d\n", cs.s, cs.E, cs.b);
    printSummary(hit, miss, evict);

    // Free cache_line
    free(cache);
    cache = NULL;

    fclose(trace);
    return 0;
}
