#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "type_utils.h"

typedef struct 
{
    void *data; // универсальный указатель
    size_t size;
    size_t capacity;
    const TypeInfo *type; // Информация о типе данных
} String;

typedef enum
{
    ERROR_SUCCESS = 0,
    ERROR_OUT_OF_MEMORY,
    ERROR_INVALID_ARGUMENT,
    ERROR_INDEX_OUT_OF_RANGE
} Error;

char* cstr_error(Error err);
void string_read(String *s);
void string_print(const char *message, String *s);
Error string_init(String *s, TypeInfo *char_size);
Error string_free(String *s);
Error string_reserve(String *s, int new_capacity);
Error string_from_cstr(String *s, const char *cstr);
Error generate_random_string(String *s, int min_len, int max_len);
Error string_concat(const String *a, const String *b, String *result);
Error string_substring(const String *s, int i, int j, String *result);
Error string_find(const String *s, const String *substr, int *result, int case_sensitive);
Error string_get_symbol(const String *s, int index, void *symbol);
Error string_set_symbol(String *s, int index, const void *symbol);
Error string_append_symbol(String *s, const void *symbol);
Error string_equal(String *s1, String *s2, int *result);

#endif