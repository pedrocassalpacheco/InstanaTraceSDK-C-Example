#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include <sys/timeb.h>

#define MAXSTRING 255
#define MAXJSON  2048

struct timeb start;
typedef struct  {
    char key[MAXSTRING] ;
    char value[MAXSTRING];
} kv;

void entry_point(char*, kv*);
void intermidiate_point(char*, kv*);
void exit_point(char*, kv*);
void populate_metadata(char*, kv*, int);
void init_trace();
uint64_t elapse_time();
