#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

typedef struct TypeInfo
{
    size_t char_size; // количество байт в символе
    
    // Функции для работы с данными
    void (*char_to_symbol)(const struct TypeInfo*, void*, char);
    int (*compare)(const void*, const void*);
    void (*copy)(void*, const void*);
} TypeInfo;

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

void string_read(String *s);
void string_print(const char *message, String *s);
void copy_char(void *dest, const void *src);
void char_to_symbol(const TypeInfo *type, void *symbol, char c);
Error string_init(String *s, TypeInfo *char_size);
Error string_free(String *s);
Error string_reserve(String *s, size_t new_capacity);
Error string_from_cstr(String *s, const char *cstr);
Error generate_random_string(String *s, int min_len, int max_len);
Error string_concat(const String *a, const String *b, String *result);
Error string_substring(const String *s, int i, int j, String *result);
Error string_find(const String *s, const String *substr, int *result, int case_sensitive);
Error string_get_symbol(const String *s, size_t index, void *symbol);
Error string_set_symbol(String *s, size_t index, const void *symbol);
Error string_append_symbol(String *s, const void *symbol);
Error string_equal(String *s1, String *s2, int *result);

#endif