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
        /* line_buf has information of operation, address, size in order. */
        /* Divide the information into three section                      */

        // Pass the memory for Instruction
        if (line_buf[0] == 'I'){
             continue;
        }
        // Corner Case : if line starts with space, passes it
        else if (line_buf[0] == ' ')
            line_buf++;

        /* Initialization of three infos. */
        op = strtok(line_buf, " ");
        addr_tmp = strtok(NULL, ",");
        size_tmp = strtok(NULL, "\n");

        /* Converts string to int */
        long long int addr = strtoll(addr_tmp, NULL, 16);
        int size = strtoll(size_tmp, NULL, 10);

        printf("%s %llx,%d\n", op, addr, size);

    }

    
    printSummary(hit, miss, evict);

    return 0;
}
