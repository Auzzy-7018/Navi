#include "var.h"
#include <stdlib.h>
#include <string.h>

#define VAR_INIT_CAP 8

/* ---------- internal helpers ---------- */

static int var_grow(var *v)
{
    size_t new_cap = v->cap ? v->cap * 2 : VAR_INIT_CAP;
    char **tmp = realloc(v->data, new_cap * sizeof(char *));
    if (!tmp)
        return -1;

    v->data = tmp;
    v->cap  = new_cap;
    return 0;
}

/* ---------- methods ---------- */

static int var_push_impl(var *v, const char *s)
{
    pthread_mutex_lock(&v->lock);

    if (v->len == v->cap) {
        if (var_grow(v) < 0) {
            pthread_mutex_unlock(&v->lock);
            return -1;
        }
    }

    char *copy = strdup(s);
    if (!copy) {
        pthread_mutex_unlock(&v->lock);
        return -1;
    }

    v->data[v->len++] = copy;

    pthread_mutex_unlock(&v->lock);
    return 0;
}

static int var_set_impl(var *v, size_t index, const char *s)
{
    pthread_mutex_lock(&v->lock);

    if (index >= v->len) {
        pthread_mutex_unlock(&v->lock);
        return -1;
    }

    char *copy = strdup(s);
    if (!copy) {
        pthread_mutex_unlock(&v->lock);
        return -1;
    }

    free(v->data[index]);
    v->data[index] = copy;

    pthread_mutex_unlock(&v->lock);
    return 0;
}

static int var_pop_impl(var *v)
{
    pthread_mutex_lock(&v->lock);

    if (v->len == 0) {
        pthread_mutex_unlock(&v->lock);
        return -1;
    }

    free(v->data[v->len - 1]);
    v->len--;

    pthread_mutex_unlock(&v->lock);
    return 0;
}

static int var_remove_impl(var *v, size_t index)
{
    pthread_mutex_lock(&v->lock);

    if (index >= v->len) {
        pthread_mutex_unlock(&v->lock);
        return -1;
    }

    free(v->data[index]);

    for (size_t i = index; i < v->len - 1; i++)
        v->data[i] = v->data[i + 1];

    v->len--;

    pthread_mutex_unlock(&v->lock);
    return 0;
}

static void var_clear_impl(var *v)
{
    pthread_mutex_lock(&v->lock);

    for (size_t i = 0; i < v->len; i++)
        free(v->data[i]);

    v->len = 0;

    pthread_mutex_unlock(&v->lock);
}

static char *var_get_impl(var *v, size_t index)
{
    pthread_mutex_lock(&v->lock);

    char *result = (index < v->len) ? v->data[index] : NULL;

    pthread_mutex_unlock(&v->lock);
    return result;
}

static size_t var_length_impl(var *v)
{
    pthread_mutex_lock(&v->lock);
    size_t n = v->len;
    pthread_mutex_unlock(&v->lock);
    return n;
}

static void var_destroy_impl(var *v)
{
    v->clear(v);

    pthread_mutex_lock(&v->lock);
    free(v->data);
    v->data = NULL;
    v->cap  = 0;
    pthread_mutex_unlock(&v->lock);

    pthread_mutex_destroy(&v->lock);
}


/*
 * Splits `input` by multi-character delimiter `delim`
 * Example:
 *   "a::b::c", "::" -> ["a", "b", "c"]
 */
int var_split(var *out, const char *input, const char *delim)
{
    if (!out || !input || !delim || !*delim)
        return -1;

    size_t delim_len = strlen(delim);
    const char *start = input;
    const char *pos;

    while ((pos = strstr(start, delim)) != NULL) {
        size_t len = (size_t)(pos - start);

        if (len > 0) {
            char buf[len + 1];
            memcpy(buf, start, len);
            buf[len] = '\0';

            if (out->push(out, buf) < 0)
                return -1;
        }

        start = pos + delim_len;
    }

    /* tail */
    if (*start) {
        if (out->push(out, start) < 0)
            return -1;
    }

    return 0;
}

/* ---------- constructor ---------- */

int var_init(var *v)
{
    v->data = NULL;
    v->len  = 0;
    v->cap  = 0;

    if (pthread_mutex_init(&v->lock, NULL) != 0)
        return -1;

    /* bind methods */
    v->push    = var_push_impl;
    v->set     = var_set_impl;
    v->pop     = var_pop_impl;
    v->remove  = var_remove_impl;
    v->clear   = var_clear_impl;
    v->get     = var_get_impl;
    v->length  = var_length_impl;
    v->destroy = var_destroy_impl;

    return 0;
}
