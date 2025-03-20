#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

typedef struct 
{
    wchar_t *data;
    size_t size;
    size_t capacity;
} String;

typedef enum
{
    ERROR_SUCCESS = 0,
    ERROR_OUT_OF_MEMORY,
    ERROR_INVALID_ARGUMENT,
    ERROR_INDEX_OUT_OF_RANGE
} Error;

Error string_init(String *s);
Error string_free(String *s);
Error string_reserve(String *s, size_t new_capacity);
Error string_from_cstr(String *s, const char *cstr);
Error generate_random_string(String *s, int min_len, int max_len);
Error string_concat(const String *a, const String *b, String *result);
Error string_substring(const String *s, int i, int j, String *result);
Error string_find(const String *s, const String *substr, int *result, int case_sensitive);
Error string_get_symbol(const String *s, size_t index, wchar_t *symbol);
Error string_set_symbol(String *s, size_t index, wchar_t symbol);
Error string_append_symbol(String *s, wchar_t symbol);

#endif