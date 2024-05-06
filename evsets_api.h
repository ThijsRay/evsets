#ifndef evsets_api_H
#define evsets_api_H

#include "public_structs.h"

int init_evsets(struct config *conf);
int find_evsets(void);
int get_num_evsets(void);
Elem *get_evset(int id);
void close_evsets(void);

#endif /* evsets_api_H */
