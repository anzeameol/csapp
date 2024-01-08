#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csim.h"
#include <getopt.h>

// ./csim -s 4 -E 1 -b 4 -t traces/long.trace
// ./csim-ref -s 4 -E 1 -b 4 -t traces/long.trace

void initCache()
{
    cache = (struct cacheLine **)malloc(sizeof(struct cacheLine *) * numSet);
    for (int i = 0; i < numSet; i++)
    {
        cache[i] = (struct cacheLine *)malloc(sizeof(struct cacheLine) * associativity);
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j].valid = 0;
        }
    }
}

void freeCache()
{
    for (int i = 0; i < numSet; i++)
    {
        free(cache[i]);
    }
    free(cache);
}

void usage()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(1);
}

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

void updateCounter(int setIndex, int unchanged)
{
    for (int i = 0; i < associativity; i++)
    {
        if (i != unchanged)
        {
            cache[setIndex][i].counter++;
        }
    }
}

void replace_LRU(int setIndex, int tag)
{
    for (int i = 0; i < associativity; i++)
    {
        if (!cache[setIndex][i].valid)
        {
            cache[setIndex][i].valid = 1;
            cache[setIndex][i].tag = tag;
            cache[setIndex][i].counter = 0;
            updateCounter(setIndex, i);
            return;
        }
    }
    evictions++;
    if (verbose)
    {
        printf(" eviction");
    }
    int maxCounter = MININT;
    int blockIndex = 0;
    for (int i = 0; i < associativity; i++)
    {
        if (cache[setIndex][i].counter > maxCounter)
        {
            maxCounter = cache[setIndex][i].counter;
            blockIndex = i;
        }
    }
    cache[setIndex][blockIndex].tag = tag;
    cache[setIndex][blockIndex].counter = 0;
    updateCounter(setIndex, blockIndex);
}

void loadCache(int setIndex, int tag)
{
    for (int i = 0; i < associativity; i++)
    {
        if (cache[setIndex][i].valid && cache[setIndex][i].tag == tag)
        {
            hits++;
            cache[setIndex][i].counter = 0;
            updateCounter(setIndex, i);
            if (verbose)
            {
                printf(" hit");
            }
            return;
        }
    }
    misses++;
    if (verbose)
    {
        printf(" miss");
    }
    replace_LRU(setIndex, tag);
}

void storeCache(int setIndex, int tag)
{
    for (int i = 0; i < associativity; i++)
    {
        if (cache[setIndex][i].valid && cache[setIndex][i].tag == tag)
        {
            hits++;
            cache[setIndex][i].counter = 0;
            updateCounter(setIndex, i);
            if (verbose)
            {
                printf(" hit");
            }
            return;
        }
    }
    misses++;
    if (verbose)
    {
        printf(" miss");
    }
    replace_LRU(setIndex, tag);
}

void simulate()
{
    FILE *trace;
    if ((trace = fopen(filePath, "r")) == NULL)
    {
        printf("cannot open the trace file\n");
        exit(1);
    }
    initCache();
    char instr;
    long long addr;
    int size;
    int tag;
    int setIndex;
    int dataSize = blockSize * numSet;
    while (fscanf(trace, " %c %llx,%d", &instr, &addr, &size) != EOF)
    {
        setIndex = (addr % dataSize) / blockSize;
        tag = addr / dataSize;
        if (verbose)
        {
            printf("%c %llx,%d", instr, addr, size);
        }
        switch (instr)
        {
        case 'I':
            break;
        case 'L':
            loadCache(setIndex, tag);
            break;
        case 'S':
            storeCache(setIndex, tag);
            break;
        case 'M':
            loadCache(setIndex, tag);
            storeCache(setIndex, tag);
            break;
        default:
            break;
        }
        if (verbose)
        {
            printf("\n");
        }
    }
    freeCache();
    fclose(trace);
}

int main(int argc, char *argv[])
{
    parseline(argc, argv);
    simulate();
    printSummary(hits, misses, evictions);
    return 0;
}
