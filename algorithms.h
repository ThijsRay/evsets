#ifndef algorithms_H
#define algorithms_H

#include "cache.h"

int naive_eviction(EvsetsElement **ptr, EvsetsElement **can, char *victim);
int naive_eviction_optimistic(EvsetsElement **ptr, EvsetsElement **can, char *victim);
int gt_eviction(EvsetsElement **ptr, EvsetsElement **can, char *victim);
int gt_eviction_any(EvsetsElement **ptr, EvsetsElement **can);
int binary_eviction(EvsetsElement **ptr, EvsetsElement **can, char *victim);

#endif /* algorithms_H */
