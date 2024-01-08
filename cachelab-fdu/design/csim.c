#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csim.h"
#include <getopt.h>

// print usage info
void usage()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(1);
}

// parse command line and get the parameters
void parseline(int argc, char **argv)
{
    int opt;
    int num = 0;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            usage();
            break;
        case 'v':
            verbose = 1;
            break;
        case 's':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            numSet = num;
            break;
        case 'E':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            associativity = num;
            break;
        case 'b':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            blockSize = num;
            break;
        case 't':
            strcpy(filePath, optarg);
            break;
        case ':':
            printf("./csim: Missing required command line argument\n");
            usage();
            break;
        case '?':
            usage();
            break;
        default:
            printf("getopt error");
            exit(1);
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    parseline(argc, argv);
    // Simulate();
    printSummary(hits, misses, evictions);
    return 0;
}
