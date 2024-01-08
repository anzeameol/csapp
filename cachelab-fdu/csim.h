#pragma once

#define MININT -2147483648

int numSet;
int associativity;
int blockSize;
char filePath[100];
int verbose = 0;

int hits = 0;
int misses = 0;
int evictions = 0;

extern char *optarg;

struct cacheLine
{
    int valid;
    int tag;
    int counter;
};

struct cacheLine **cache;

void initCache();
void freeCache();
void usage();
void parseline(int argc, char **argv);
void updateCounter(int setIndex, int unchanged);
void replace_LRU(int setIndex, int tag);
void loadCache(int setIndex, int tag);
void storeCache(int setIndex, int tag);
void simulate();