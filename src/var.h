#ifndef VAR_H
#define VAR_H

#include <stddef.h>
#include <pthread.h>

/* forward declaration */
typedef struct var var;

/* ---------- object ---------- */

struct var {
    char **data;
    size_t len;
    size_t cap;
    pthread_mutex_t lock;

    /* methods */
    int    (*push)(var *self, const char *s);
    int    (*set)(var *self, size_t index, const char *s);
    int    (*pop)(var *self);
    int    (*remove)(var *self, size_t index);
    void   (*clear)(var *self);
    char * (*get)(var *self, size_t index);
    size_t (*length)(var *self);
    void   (*destroy)(var *self);
};

/* ---------- constructor ---------- */

int var_init(var *v);
int var_split(var *out, const char *input, const char *delim);

/* ---------- macros (NO LOCKING) ---------- */

#define VAR_AT(v, i)   ((v)->data[(i)])
#define VAR_LEN(v)     ((v)->len)

#define VAR_FOREACH(v, i) \
    for (size_t i = 0; i < (v)->len; i++)

#endif /* VAR_H */
