/* Kim Seonghoon, y2016142212 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cachelab.h"

int main(int argc, char* argv[]) {
    int hit = 0;
    int miss = 0;
    int evict = 0;

    FILE* trace = fopen(argv[8], "r");           // open trace file
    char* line_buf = malloc(100);  // line buffer

    char* op = malloc(sizeof(char)); 
    /* address in string. 16 bytes allocated */
    char* addr_tmp = malloc(16);  
    /* size in string. */
    char* size_tmp = malloc(sizeof(int)); 
    

   
    while (fgets(line_buf, 100, trace)!=NULL) {
        
    }

    
    printSummary(hit, miss, evict);

    return 0;
}
