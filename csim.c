/* Kim Seonghoon, y2016142212 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cachelab.h"

typedef struct trace_line {
    unsigned long int addr;  // Address. uint64
    int size;
    char op;   // L, S or M
} trace_line;  // struct for trace line.

typedef struct tag_info {
    long int tag;  // tag bit
    struct tag_info* prev;
    struct tag_info* next;  // Linked List
} tag_info;

tag_info* InsertNode(long int x, struct tag_info* head);
void DeleteNode(struct tag_info* tag);
void freeList(struct tag_info* head);

int main(int argc, char* argv[]) {
    int hit = 0;
    int miss = 0;
    int evict = 0;

    int index;     // index
    long int tag;  // tag

    int set;       // set index bits
    int lines;     // Associativity. # of lines per set
    int b_offset;  // block offset

    /* Extract cache structure from arguments */
    int op;              // option code
    int verbose = 0;     // verbose mode on? off?
    char file_name[20];  // buffer for file name
    
    // Read option code
    while ((op = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch (op) {
            case 'v':   // verbose mode
                verbose = 1;
                break;
            case 's':   // set bits
                set = atoi(optarg);
                break;
            case 'E':   // Associativity. # of lines
                lines = atoi(optarg);
                break;
            case 'b':   // block offset bits.
                b_offset = atoi(optarg);
                break;
            case 't':   // file name option
                memcpy(file_name, optarg, 20);
                break;
            default:   // Wrong parameter
                fprintf(stderr, "Wrong parameter\n");
                break;
        }
    }

    int total_set = 1 << set;  // total_set # = 2^s

    /* Cache Line implementation */
    typedef struct cache_line {
        tag_info* base;
        int valid_bit;  // Valid bit.
    } cache_line;

    /* Cache Line Memory allocation */
    struct cache_line* cache = malloc(sizeof(struct cache_line) * total_set);
    // initializes the total cache = cache_line * set

    struct tag_info* tmp_base;  // buffer for Linked List

    /* Cache Initialization */

    for (int i = 0; i < total_set; ++i) {
        cache[i].valid_bit = 0;
        /* Memory allocation. base of the tag list */
        cache[i].base = malloc(sizeof(tag_info));
        /* tmp for LinkedList*/
        tmp_base = cache[i].base;
        tmp_base->prev = NULL;  // head -> prev = NULL;

        /* Linked List. Malloc next node */
        for (int j = 0; j < lines - 1; ++j) {
            tmp_base->next = malloc(sizeof(tag_info));
            tmp_base->next->prev = tmp_base;
            // if tag field is empty, tag = -1
            tmp_base->tag = -1;
            tmp_base = tmp_base->next;
        }
        // Now, tmp_base = tail
        tmp_base->tag = -1;
        tmp_base->next = NULL;  // tail -> next = NULL
    }

    int result = 0;  // Checks if file read reached end or not.

    FILE* trace = fopen(file_name, "r");  // open trace file
    if (trace == NULL)                    // print error if file is invalid
        fprintf(stderr, "Invaild Trace File.\n");

    /* Every Line Buffer*/
    struct trace_line* t = malloc(sizeof(struct trace_line));

    while (1) {
        // Read File Line by Line
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
        index = t->addr << (64 - set - b_offset) >> (64 - set);
        tag = t->addr >> (set + b_offset);

        /////////////////////////////////////////////////////
        /////// Important part. Cache Logics below //////////
        /////////////////////////////////////////////////////

        /* Temporary hit, miss variables for checking. 0 or 1 Values */
        int tmp_hit = 0;
        int tmp_miss = 0;
        int tmp_evict = 0;

        /* Cache Walk. */
        /* Index is filled with some data */
        if (cache[index].valid_bit == 1) {
            tmp_base = cache[index].base;
            /* Now Checks the tag exists in the cache line */
            while (tmp_base != NULL) {
                if (tmp_base->tag == tag) {
                    tmp_hit = 1;
                    // Realign the list for LRU algorithm
                    // duplicate node and set as head
                    cache[index].base = InsertNode(tag, cache[index].base);
                    // Delete the current hit node
                    DeleteNode(tmp_base);
                    break;
                }
                tmp_base = tmp_base->next;
            }
            if (tmp_hit != 1) {
                /* Miss */
                tmp_miss = 1;
                tmp_base = cache[index].base;
                /* Find LRU node (tail) */
                while (tmp_base->next != NULL) {
                    tmp_base = tmp_base->next;
                }
                /* Now, tmp_base : tail */
                /* if tail node is not empty, eviction = 1 */
                if (tmp_base->tag != -1) tmp_evict = 1;
                cache[index].base = InsertNode(tag, cache[index].base);
                DeleteNode(tmp_base);
            }
            /* Miss, Fill the empty index */
        } else {
            tmp_base = cache[index].base;
            tmp_miss = 1;
            cache[index].valid_bit = 1;
            cache[index].base = InsertNode(tag, cache[index].base);
            // Delete next node
            DeleteNode(cache[index].base->next);
        }

        // If memory access type is 'M',
        // store(after load) is always hit
        if (t->op == 'M') ++tmp_hit;

        if (verbose == 1) {
            printf("%c %lx,%d ", t->op, t->addr, t->size);
            if (tmp_miss == 1) printf("miss ");
            if (tmp_evict == 1) printf("eviction ");
            if (tmp_hit == 1)
                printf("hit ");
            else if (tmp_hit == 2)
                printf("hit hit ");

            printf("\n");
        }

        hit += tmp_hit;
        miss += tmp_miss;
        evict += tmp_evict;
    }

    for (int i = 0; i < total_set; ++i) freeList(cache[i].base);

    free(cache);
    cache = NULL;

    free(t);
    t = NULL;

    printSummary(hit, miss, evict);

    fclose(trace);
    return 0;
}

// Insert tag node at front
// return new head
tag_info* InsertNode(long int x, struct tag_info* head) {
    // Generate New Node with fresh data
    struct tag_info* new = malloc(sizeof(struct tag_info));
    new->tag = x;
    // Rebase
    new->next = head;
    head->prev = new;
    new->prev = NULL;
    // Now, NULL -> new -> head -> ...
    return new;
}

// Delete node and realign
void DeleteNode(struct tag_info* tag) {
    /* First, realign the data */
    // if input is tail
    if (tag->next == NULL) {
        // prev -> tag -> NULL
        // prev     ->    NULL
        tag->prev->next = NULL;
        // if input is in the middle
    } else {
        // prev -> <- tag -> <- next
        // prev     -> <-    next
        tag->prev->next = tag->next;
        tag->next->prev = tag->prev;
    }

    /* Then, delete the node */
    tag->next = NULL;
    tag->prev = NULL;
    free(tag);
}

// Free List
void freeList(struct tag_info* head) {
    struct tag_info* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
        tmp = NULL;
    }
}