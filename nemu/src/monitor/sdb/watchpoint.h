#ifndef WITHPOINT_H
#define WITHPOINT_H

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  bool use_flag;
  char expr[50];
  int old_value;
  int new_value;
} WP;

extern WP wp_pool[NR_WP];
static WP *head __attribute__((unused));
static WP *free_ __attribute__((unused));

void init_wp_pool();
WP* new_wp();
void free_wp(WP *wp);

#endif