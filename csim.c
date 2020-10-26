/* Kim Seonghoon, y2016142212 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cachelab.h"

int main(int argc, char* argv[]) {
    int hit = 0;
    int miss = 0;
    int evict = 0;

    typedef struct {
        char op;
        unsigned long int addr;
        int size;
    } trace_line;  // struct for trace line.

    typedef struct {
        int s;       // 2^s is the number of sets
        int E;       // Associativity. # of lines per set
        int b;       // 2^b is the block size
    } cache_struct;  // Cache structure

    typedef struct {
        int index;
        int tag;
        int b_offset;  // block offset
    } cache_info;

    FILE* trace = fopen(argv[8], "r");  // open trace file
    if (trace == NULL)                  // print error if file is invalid
        fprintf(stderr, "Invaild Trace File.");

    int result = 0;

    trace_line t;
    cache_struct cs;
    cache_info ci;

    /* Extract cache structure from arguments */
    cs.s = atoi(argv[2]);
    cs.E = atoi(argv[4]);
    cs.b = atoi(argv[6]);

    while (1) {
        result = fscanf(trace, " %s %lx, %d", &t.op, &t.addr, &t.size);
        if (result == EOF) break;  // END of trace file. break.

        // abandon instruction trace
        if (t.op == 'I') continue;

        /*                  *** Address Structure ***
        |        (64-s-b) bits       |      s bits     |   b bits   |
        |           tag              |      index      |  b_offset  |
        */

        /* Extract Cache Information from address*/
        

        
        printf("%c %lx, %d\n", t.op, t.addr, t.size);
        printf("Cache Info. index = %d, tag = %d, b_offset = %d\n", ci.index, ci.tag, ci.b_offset);
    }

    printf("Cache structure. s=%d, E=%d, b=%d\n", cs.s, cs.E, cs.b);
    printSummary(hit, miss, evict);

    return 0;
}
