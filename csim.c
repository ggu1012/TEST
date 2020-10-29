/* Kim Seonghoon, y2016142212 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cachelab.h"

typedef struct trace_line{
    char op;                 // L, S or M
    unsigned long int addr;  // Address. uint64
    int size;
} trace_line;  // struct for trace line.

int main(int argc, char* argv[]) {

    int hit = 0;
    int miss = 0;
    int evict = 0;

    int index, tag;
    int s, E, b;
    
    /* Extract cache structure from arguments */
    s = atoi(argv[2]);
    E = atoi(argv[4]);
    b = atoi(argv[6]);

    int total_set = s << 1; // total_set # = 2^s

    /* Cache Line implementation */
    typedef struct cache_line{
        long int tag[E];     // Stored tags
        int LRU[E]; // if it is LRU tag, LRU[i] = 1;
        int valid_bit;     // Valid bit.
    } cache_line;

    /* Cache Initialization w/ Memory allocation */
    cache_line* cache = malloc(sizeof(cache_line) * total_set); 
    // initializes the total cache = cache_line * set
    
    /* Every Line Buffer*/
    trace_line* t = malloc(sizeof(trace_line));


    FILE* trace = fopen(argv[8], "r");  // open trace file
    if (trace == NULL)                  // print error if file is invalid
        fprintf(stderr, "Invaild Trace File.\n");


    for (int i = 0; i < total_set; ++i) {
        for (int j = 0; j < E; ++j)
            cache[i].tag[j] = -1;  // tag field is empty if tag == -1
        cache[i].valid_bit = 0;
    }
    int result = 0;  // Checks if file read reached end or not.

    while (1) {
        result = fscanf(trace, " %s %lx, %d", &t->op, &t->addr, &t->size);

        // END of trace file. break.
        if (result == EOF) break;

        // abandon instruction trace
        if (t->op == 'I') continue;

        /*
                            *** Address Implementation ***
        |        (64-s-b) bits       |      s bits     |   b bits   |
        |           tag              |      index      |  b_offset  |

        */

        /* Extract Cache Information from address*/
        index = t->addr << (64 - s - b) >> (64 - s);
        tag = t->addr >> (s + b);

        /////////////////////////////////////////////////////
        /////// Important part. Cache Logics below //////////
        /////////////////////////////////////////////////////

        /* Temporary hit, miss variables for checking. 0 or 1 Values */
        int tmp_hit = 0;
        int tmp_miss = 0;
        int tmp_evict = 0;

        /* First, checkout the memory access type */
        switch (t->op) {
            /* Load. */
            case 'L':
                /* Cache Walk. */
                /* Index is filled with some data */
                if (cache[index].valid_bit == 1) {
                    /* Now Checks the tag exists in the cache line */
                    for (int tmp_line = 0; tmp_line < E; ++tmp_line) {
                        /* Hit ! */
                        if (cache[index].tag[tmp_line] == tag) {
                            tmp_hit = 1;
                            /* if Hit is LRU component, change LRU bit location*/
                            for(int i = 0; i < E; ++i)
                                if(cache[index.LRU[i]])
                                cache[index].LRU[i] = 0;
                            
                            break;
                        }
                    }
                    if (tmp_hit != 1) {
                        /* Miss */
                        tmp_miss = 1;
                        tmp_evict = 1;  // First, set tmp_evict = 1
                        /* Checks where cache.RUinfo == E - 1,
                         * which is Least Recently Used */
                        for (int tmp_line = 0; tmp_line < E; ++tmp_line) {
                            if (cache[index].RUinfo[tmp_line] == E - 1) {
                                /* If tag field is empty, Eviction does not
                                 * happened */
                                if (cache[index].tag[tmp_line] == -1)
                                    tmp_evict = 0;
                                /* tag update */
                                cache[index].tag[tmp_line] = tag;
                                /* Update Recently Used data */
                                for (int i = 0; i < E; ++i)
                                    ++cache[index].RUinfo[i];
                                cache[index].RUinfo[tmp_line] = 0;
                                break;
                            }
                        }
                    }
                    /* Miss, Fill the empty index */
                } else {
                    tmp_miss = 1;
                    cache[index].tag[0] = tag;
                    cache[index].valid_bit = 1;
                    /* Set RUinfo array as {0, 1, ..., E-1} */
                    for (int i = 0; i < E; ++i) cache[index].RUinfo[i] = i;
                }
            
            /* Store. */
            case 'S':;

            /* Modify. Load and Store in order */
            case 'M':;
            default:;
        }

        if (tmp_hit == 1)
            printf("%c %lx,%d hit\n", t->op, t->addr, t->size);
        else if (tmp_miss == 1) {
            if (tmp_evict == 1)
                printf("%c %lx,%d miss eviction\n", t->op, t->addr, t->size);
            else
                printf("%c %lx,%d miss\n", t->op, t->addr, t->size);
        }

        hit += tmp_hit;
        miss += tmp_miss;
        evict += tmp_evict;
    }

    printf("Cache structure. s=%d, E=%d, b=%d\n", s, E, b);
    printSummary(hit, miss, evict);

    // Free cache_line
    free(cache);
    cache = NULL;

    free(t);
    t = NULL;

    fclose(trace);
    return 0;
}
