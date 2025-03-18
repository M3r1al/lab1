#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

typedef struct 
{
    char *data;
    size_t size;
    size_t capacity;
} String;

void string_init(String *s);
void string_free(String *s);
int string_reserve(String *s, size_t new_capacity);
void string_from_cstr(String *s, const char *cstr);
void generate_random_string(String *s, int min_len, int max_len);
void string_concat(const String *a, const String *b, String *result);
void string_substring(const String *s, int i, int j, String *result);
void string_find(const String *s, const String *substr, int *result, int case_sensitive);

#endif