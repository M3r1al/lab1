#include <stdlib.h>
#include <string.h>
#include "type_utils.h"

TypeInfo* create_char_type_info()
{
    TypeInfo* type = malloc(sizeof(TypeInfo));
    type->char_size = 1;
    type->char_to_symbol = char_to_symbol;
    type->copy = copy_char;
    return type;
}

TypeInfo* create_wchar_type_info()
{
    TypeInfo* type = malloc(sizeof(TypeInfo));
    type->char_size = sizeof(wchar_t);
    type->char_to_symbol = wchar_to_symbol;
    type->copy = copy_wchar;
    return type;
}

void copy_char(void *dest, const void *src)
{
    *(char*)dest = *(const char*)src;
}

void copy_wchar(void *dest, const void *src)
{
    *(wchar_t*)dest = *(const wchar_t*)src;
}

void char_to_symbol(const TypeInfo *type, void *symbol, void *data)
{
    char c = *(char*)data;

    // заполняем символ нулями
    memset(symbol, 0, type->char_size);
    
    // Копируем исходный char в первый байт
    *(char*)symbol = c;
}

void wchar_to_symbol(const TypeInfo *type, void *symbol, void *data)
{
    wchar_t c = *(wchar_t*)data;
    memset(symbol, 0, type->char_size);
    *(wchar_t*)symbol = (wchar_t)c;
}