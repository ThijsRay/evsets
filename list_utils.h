#ifndef list_utils_H
#define list_utils_H

#include "cache.h"
#include "micro.h"
#include <stdlib.h>

int list_length(EvsetsElement *ptr);
EvsetsElement *list_pop(EvsetsElement **ptr);
EvsetsElement *list_shift(EvsetsElement **ptr);
void list_push(EvsetsElement **ptr, EvsetsElement *e);
void list_append(EvsetsElement **ptr, EvsetsElement *e);
void list_split(EvsetsElement *ptr, EvsetsElement **chunks, int n);
EvsetsElement *list_slice(EvsetsElement **ptr, size_t s, size_t e);
EvsetsElement *list_get(EvsetsElement **ptr, size_t n);
void list_concat(EvsetsElement **ptr, EvsetsElement *chunk);
void list_from_chunks(EvsetsElement **ptr, EvsetsElement **chunks, int avoid, int len);
void list_set_id(EvsetsElement *ptr, int id);
void print_list(EvsetsElement *ptr);

//void initialize_random_list(Elem *ptr, ul offset, ul sz, Elem *base);
void initialize_list(EvsetsElement *ptr, ul sz, ul offset);
void pick_n_random_from_list(EvsetsElement *src, ul stride, ul sz, ul offset, ul n);
void rearrange_list(EvsetsElement **ptr, ul stride, ul sz, ul offset);
void generate_conflict_set(EvsetsElement **ptr, EvsetsElement **out);

#endif /* list_utils_H */
