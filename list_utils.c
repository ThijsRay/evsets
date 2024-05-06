#include "list_utils.h"
#include <stdio.h>
#include <stdlib.h>

extern struct evsets_config conf;

int list_length(EvsetsElement *ptr) {
  int l = 0;
  while (ptr) {
    l = l + 1;
    ptr = ptr->next;
  }
  return l;
}

/* add element to the head of the list */
void list_push(EvsetsElement **ptr, EvsetsElement *e) {
  if (!e) {
    return;
  }
  e->prev = NULL;
  e->next = *ptr;
  if (*ptr) {
    (*ptr)->prev = e;
  }
  *ptr = e;
}

/* add element to the end of the list */
void list_append(EvsetsElement **ptr, EvsetsElement *e) {
  EvsetsElement *tmp = *ptr;
  if (!e) {
    return;
  }
  if (!tmp) {
    *ptr = e;
    return;
  }
  while (tmp->next) {
    tmp = tmp->next;
  }
  tmp->next = e;
  e->prev = tmp;
  e->next = NULL;
}

/* remove and return last element of list */
EvsetsElement *list_shift(EvsetsElement **ptr) {
  EvsetsElement *tmp = (ptr) ? *ptr : NULL;
  if (!tmp) {
    return NULL;
  }
  while (tmp->next) {
    tmp = tmp->next;
  }
  if (tmp->prev) {
    tmp->prev->next = NULL;
  } else {
    *ptr = NULL;
  }
  tmp->next = NULL;
  tmp->prev = NULL;
  return tmp;
}

/* remove and return first element of list */
EvsetsElement *list_pop(EvsetsElement **ptr) {
  EvsetsElement *tmp = (ptr) ? *ptr : NULL;
  if (!tmp) {
    return NULL;
  }
  if (tmp->next) {
    tmp->next->prev = NULL;
  }
  *ptr = tmp->next;
  tmp->next = NULL;
  tmp->prev = NULL;
  return tmp;
}

void list_split(EvsetsElement *ptr, EvsetsElement **chunks, int n) {
  if (!ptr) {
    return;
  }
  int len = list_length(ptr), k = len / n, i = 0, j = 0;
  while (j < n) {
    i = 0;
    chunks[j] = ptr;
    if (ptr) {
      ptr->prev = NULL;
    }
    while (ptr != NULL && ((++i < k) || (j == n - 1))) {
      ptr = ptr->next;
    }
    if (ptr) {
      ptr = ptr->next;
      if (ptr && ptr->prev) {
        ptr->prev->next = NULL;
      }
    }
    j++;
  }
}

EvsetsElement *list_get(EvsetsElement **ptr, size_t n) {
  EvsetsElement *tmp = *ptr;
  size_t i = 0;
  if (!tmp) {
    return NULL;
  }
  while (tmp && i < n) {
    tmp = tmp->next;
    i = i + 1;
  }
  if (!tmp) {
    return NULL;
  }
  if (tmp->prev) {
    tmp->prev->next = tmp->next;
  } else {
    *ptr = tmp->next;
  }
  if (tmp->next) {
    tmp->next->prev = tmp->prev;
  }
  tmp->prev = NULL;
  tmp->next = NULL;
  return tmp;
}

EvsetsElement *list_slice(EvsetsElement **ptr, size_t s, size_t e) {
  EvsetsElement *tmp = (ptr) ? *ptr : NULL, *ret = NULL;
  size_t i = 0;
  if (!tmp) {
    return NULL;
  }
  while (i < s && tmp) {
    tmp = tmp->next;
    i = i + 1;
  }
  if (!tmp) {
    return NULL;
  }
  // set head of new list
  ret = tmp;
  while (i < e && tmp) {
    tmp = tmp->next;
    i = i + 1;
  }
  if (!tmp) {
    return NULL;
  }
  // cut slice and return
  if (ret->prev) {
    ret->prev->next = tmp->next;
  } else {
    *ptr = tmp->next;
  }
  if (tmp->next)
    tmp->next->prev = ret->prev;
  ret->prev = NULL;
  tmp->next = NULL;
  return ret;
}

/* concat chunk of elements to the end of the list */
void list_concat(EvsetsElement **ptr, EvsetsElement *chunk) {
  EvsetsElement *tmp = (ptr) ? *ptr : NULL;
  if (!tmp) {
    *ptr = chunk;
    return;
  }
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = chunk;
  if (chunk) {
    chunk->prev = tmp;
  }
}

void list_from_chunks(EvsetsElement **ptr, EvsetsElement **chunks, int avoid, int len) {
  int next = (avoid + 1) % len;
  if (!(*ptr) || !chunks || !chunks[next]) {
    return;
  }
  // Disconnect avoided chunk
  EvsetsElement *tmp = chunks[avoid];
  if (tmp) {
    tmp->prev = NULL;
  }
  while (tmp && tmp->next != NULL && tmp->next != chunks[next]) {
    tmp = tmp->next;
  }
  if (tmp) {
    tmp->next = NULL;
  }
  // Link rest starting from next
  tmp = *ptr = chunks[next];
  if (tmp) {
    tmp->prev = NULL;
  }
  while (next != avoid && chunks[next] != NULL) {
    next = (next + 1) % len;
    while (tmp && tmp->next != NULL && tmp->next != chunks[next]) {
      if (tmp->next) {
        tmp->next->prev = tmp;
      }
      tmp = tmp->next;
    }
    if (tmp) {
      tmp->next = chunks[next];
    }
    if (chunks[next]) {
      chunks[next]->prev = tmp;
    }
  }
  if (tmp) {
    tmp->next = NULL;
  }
}

void print_list(EvsetsElement *ptr) {
  if (!ptr) {
    printf("(empty)\n");
    return;
  }
  while (ptr != NULL) {
    printf("%p ", (void *)ptr);
    ptr = ptr->next;
  }
  printf("\n");
}

void initialize_list(EvsetsElement *src, ul sz, ul offset) {
  unsigned int j = 0;
  for (j = 0; j < (sz / sizeof(EvsetsElement)) - offset; j++) {
    src[j].set = -2;
    src[j].delta = 0;
    src[j].prev = NULL;
    src[j].next = NULL;
  }
}

void pick_n_random_from_list(EvsetsElement *ptr, ul stride, ul sz, ul offset, ul n) {
  unsigned int count = 1, i = 0;
  unsigned int len = ((sz - (offset * sizeof(EvsetsElement))) / stride);
  EvsetsElement *e = ptr;
  e->prev = NULL;
  e->set = -1;
  ul *array = (ul *)calloc(len, sizeof(ul));
  for (i = 1; i < len - 1; i++) {
    array[i] = i * (stride / sizeof(EvsetsElement));
  }
  for (i = 1; i < len - 1; i++) {
    size_t j = i + rand() / (RAND_MAX / (len - i) + 1);
    int t = array[j];
    array[j] = array[i];
    array[i] = t;
  }
  for (i = 1; i < len && count < n; i++) {
    if (ptr[array[i]].set == -2) {
      e->next = &ptr[array[i]];
      ptr[array[i]].prev = e;
      ptr[array[i]].set = -1;
      e = e->next;
      count++;
    }
  }
  free(array);
  e->next = NULL;
}

void rearrange_list(EvsetsElement **ptr, ul stride, ul sz, ul offset) {
  unsigned int len = (sz / sizeof(EvsetsElement)) - offset, i = 0;
  EvsetsElement *p = *ptr;
  if (!p) {
    return;
  }
  unsigned int j = 0, step = stride / sizeof(EvsetsElement);
  for (i = step; i < len - 1; i += step) {
    if (p[i].set < 0) {
      p[i].set = -2;
      p[i].prev = &p[j];
      p[j].next = &p[i];
      j = i;
    }
  }
  p[0].prev = NULL;
  p[j].next = NULL;
  while (p && p->set > -1) {
    p = p->next;
  }
  *ptr = p;
  if (p) {
    p->set = -2;
    p->prev = NULL;
  }
}

void list_set_id(EvsetsElement *ptr, int id) {
  while (ptr) {
    ptr->set = id;
    ptr = ptr->next;
  }
}

void generate_conflict_set(EvsetsElement **ptr, EvsetsElement **out) {
  EvsetsElement *candidate = NULL, *res = NULL;
  int ret = 0;
  while (*ptr) // or while size |out| == limit
  {
    candidate = list_pop(ptr);
    if (conf.ratio > 0.0) {
      ret = tests(*out, (char *)candidate, conf.rounds, conf.threshold, conf.ratio, conf.traverse);
    } else {
      ret = tests_avg(*out, (char *)candidate, conf.rounds, conf.threshold, conf.traverse);
    }
    if (!ret) {
      // no conflict, add element
      list_push(out, candidate);
    } else {
      // conflict, candidate goes to list of victims
      list_push(&res, candidate);
    }
  }
  *ptr = res;
}
