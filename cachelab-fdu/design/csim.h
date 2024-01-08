#pragma once

#define MININT -2147483648

// cache parameters
int numSet;
int associativity;
int blockSize;
char filePath[100];
int verbose = 0;

// final results
int hits = 0;
int misses = 0;
int evictions = 0;

// will be set in getopt() function
extern char *optarg;

// you can define functions here
void usage();
void parseline(int argc, char **argv);
