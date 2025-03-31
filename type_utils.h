#ifndef TYPE_UTILS_H
#define TYPE_UTILS_H

#include <stddef.h>

typedef struct TypeInfo
{
    size_t char_size; // количество байт в символе
    
    // Функции для работы с данными
    void (*char_to_symbol)(const struct TypeInfo*, void*, void*);
    int (*compare)(const void*, const void*);
    void (*copy)(void*, const void*);
} TypeInfo;

TypeInfo* create_char_type_info();
TypeInfo* create_wchar_type_info();
void copy_char(void *dest, const void *src);
void copy_wchar(void *dest, const void *src);
void char_to_symbol(const TypeInfo *type, void *symbol, void *c);
void wchar_to_symbol(const TypeInfo *type, void *symbol, void *c);
void destroyTypeInfo(TypeInfo* type); // TODO

#endif // TYPE_UTILS_H