/* Author : Devan SOHIER, 2021 */

#ifndef READ_FILE
#define READ_FILE
#define MAX_SIZE 100
#define MAX_LEN 100

#include "LRGrammar.h"

typedef struct file_read{
  grammar G;
  table t;
} file_read;

file_read read_file(const char *);

#endif
