/* Author: Devan SOHIER, 2021 */

#ifndef LRGRAMMAR
#define LRGRAMMAR

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct rule{
  char lhs;
  signed char *rhs;
} rule;

typedef struct grammar{
  char start;
  size_t size;
  rule *rules;
} grammar;

void print_grammar(grammar);

typedef struct table{
  short nblines;
  signed char *trans;
} table;

void print_table(table, grammar);

#endif
