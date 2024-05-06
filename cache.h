#ifndef cache_H
#define cache_H

#include <stdint.h>
#include <stdlib.h>

#ifdef THREAD_COUNTER
#include <pthread.h>
#endif

#include "private_structs.h"

#ifdef THREAD_COUNTER
static pthread_t thread;
static struct params_t params;

void *counter_thread();
static inline uint64_t clock_thread();
int create_counter();
void destroy_counter();
#endif /* THREAD_COUNTER */

void traverse_list_skylake(EvsetsElement *ptr);
void traverse_list_haswell(EvsetsElement *ptr);
void traverse_list_simple(EvsetsElement *ptr);
void traverse_list_asm_skylake(EvsetsElement *ptr);
void traverse_list_asm_haswell(EvsetsElement *ptr);
void traverse_list_asm_simple(EvsetsElement *ptr);
void traverse_list_rrip(EvsetsElement *ptr);
void traverse_list_to_n(EvsetsElement *ptr, int n);
void traverse_list_time(EvsetsElement *ptr, void (*trav)(EvsetsElement *));

int test_set(EvsetsElement *ptr, char *victim, void (*trav)(EvsetsElement *));
int tests(EvsetsElement *ptr, char *victim, int rep, int threshold, float ratio,
          void (*trav)(EvsetsElement *));
int tests_avg(EvsetsElement *ptr, char *victim, int rep, int threshold, void (*trav)(EvsetsElement *));
int test_and_time(EvsetsElement *ptr, int rep, int threshold, int ways, void (*trav)(EvsetsElement *));

int calibrate(char *victim, struct evsets_config *conf);

#endif /* cache_H */
