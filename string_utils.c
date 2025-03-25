#include "string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>

// Инициализация пустой строки
Error string_init(String *s, size_t char_size) 
{
    if (!s || char_size <= 0)
        return ERROR_INVALID_ARGUMENT;
    
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
    s->char_size = char_size;
    return ERROR_SUCCESS;
}

// Освобождение памяти строки
Error string_free(String *s) 
{
    if (!s)
        return ERROR_INVALID_ARGUMENT;
    
    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
    return ERROR_SUCCESS;
}

// Изменение ёмкости строки
Error string_reserve(String *s, size_t new_capacity) 
{
    if (!s)
        return ERROR_INVALID_ARGUMENT;
    
    if (new_capacity <= s->capacity)
        return ERROR_SUCCESS;
    
    void *new_data = realloc(s->data, new_capacity * s->char_size);
    if (!new_data)
        return ERROR_OUT_OF_MEMORY; // Ошибка выделения памяти
    
    s->data = new_data;
    s->capacity = new_capacity;
    return ERROR_SUCCESS;
}

// Создание строки из C-строки
Error string_from_cstr(String *s, const char *cstr) 
{
    if (!s || !cstr || s->char_size != 1)
        return ERROR_INVALID_ARGUMENT;
    
    // Очистка предыдущего содержимого
    Error err = string_free(s);
    if (err != ERROR_SUCCESS)
        return err;
    size_t len = strlen(cstr) + 1;
    err = string_reserve(s, len);
    if (err != ERROR_SUCCESS)
        return err;
    
    memcpy(s->data, cstr, len);
    
    s->size = strlen(cstr);
    return ERROR_SUCCESS;
}

// Генерация случайной строки
Error generate_random_string(String *s, int min_len, int max_len) 
{
    if (!s)
        return ERROR_INVALID_ARGUMENT;
    
    int len = min_len + rand() % (max_len - min_len + 1);
    Error err = string_reserve(s, len + 1);
    if (err != ERROR_SUCCESS)
        return err;
    
    // Символы для генерации
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    for (int i = 0; i < len; i++)
    {
        char c = charset[rand() % (sizeof(charset) - 1)]; // -1 для исключения завершающего нуля
        memcpy((char*)s->data + (i * s->char_size), &c, 1);
    }
    
    memset((char*)s->data + (len * s->char_size), 0, s->char_size);
    s->size = len;
    return ERROR_SUCCESS;
}

// Конкатенация строк
Error string_concat(const String *a, const String *b, String *result) 
{
    if (!a || !b || a->char_size != b->char_size)
        return ERROR_INVALID_ARGUMENT;
    
    // Очистка предыдущего результата
    Error err = string_free(result);
    if (err != ERROR_SUCCESS)
        return err;
    
    size_t total_size = a->size + b->size;
    err = string_reserve(result, total_size + 1);
    if (err != ERROR_SUCCESS)
        return err;
    
    memcpy(result->data, a->data, a->size * a->char_size);
    memcpy((char*)result->data + (a->size * a->char_size), b->data, b->size * b->char_size);
    result->size = total_size;
    return ERROR_SUCCESS;
}

// Получение подстроки
Error string_substring(const String *s, int i, int j, String *result) 
{
    if (!s || i > j)
        return ERROR_INVALID_ARGUMENT;
    
    // Обработка границ индексов
    if ((int)s->size <= j || i < 0)
        return ERROR_INDEX_OUT_OF_RANGE; // Или все же лучше тоже как неверный аргумент?
    
    Error err = string_free(result);
    if (err != ERROR_SUCCESS)
        return err;
    
    size_t substring_size = j - i + 1;
    err = string_reserve(result, substring_size + 1);
    if (err != ERROR_SUCCESS)
        return err;
    
    for (size_t k = 0; k < substring_size; k++)
    {
        size_t src_offset = (i + k) * s->char_size;
        size_t dst_offset = k * result->char_size;
        memcpy((char*)result->data + dst_offset, (char*)s->data + src_offset, s->char_size);
    }
        
    memset((char*)result->data + substring_size * result->char_size, 0, result->char_size);
    result->size = substring_size;
    return ERROR_SUCCESS;
}

// Поиск подстроки с учетом регистра
Error string_find(const String *s, const String *substr, int *result, int case_sensitive) 
{
    if (!s || !substr || !result || s->char_size != substr->char_size)
        return ERROR_INVALID_ARGUMENT;
    
    // Инициализация результата
    *result = -1; 
    
    if (substr->size == 0 || substr->size > s->size)
        return ERROR_SUCCESS; // Невозможный случай
    
    // Перебор возможных позиций
    for (size_t i = 0; i <= s->size - substr->size; i++)
    { 
        int found = 1;

        // Сравнение символов
        for (size_t j = 0; j < substr->size; j++)
        { 
            size_t s_offset = (i + j) * s->char_size;
            size_t substr_offset = j * substr->char_size;

            // Временные копии для преобразования регистра
            void *a = malloc(s->char_size);
            void *b = malloc(substr->char_size);
            memcpy(a, (char*)s->data + s_offset, s->char_size);
            memcpy(b, (char*)substr->data + substr_offset, substr->char_size);

            // Преобразование к нижнему регистру
            if (!case_sensitive)
            { 
                // char
                if (s->char_size == 1)
                {
                    *(char*)a = towlower(*(char*)a);
                    *(char*)b = towlower(*(char*)b);
                }
            }
            
            if (memcmp(a, b, s->char_size) != 0)
            {
                found = 0;
                break;
            }
        }
        if (found)
        {
            *result = i;
            return ERROR_SUCCESS;
        }
    }
    return ERROR_INDEX_OUT_OF_RANGE;
}

Error string_get_symbol(const String *s, size_t index, void *symbol)
{
    if (!s || !symbol)
        return ERROR_INVALID_ARGUMENT;
    if (index >= s->size)
        return ERROR_INDEX_OUT_OF_RANGE; // Такой же вопрос про тип ошибки
    
    memcpy(symbol, (char*)s->data + (index * s->char_size), s->char_size);
    return ERROR_SUCCESS;
}

Error string_set_symbol(String *s, size_t index, const void *symbol)
{
    if (!s || !symbol)
        return ERROR_INVALID_ARGUMENT;
    if (index >= s->size)
        return ERROR_INDEX_OUT_OF_RANGE; // Аналогично, это к аргументу или ошибке с индексом относится?
    
    memcpy((char*)s->data + (index * s->char_size), symbol, s->char_size);
    return ERROR_SUCCESS;
}

Error string_append_symbol(String *s, const void *symbol)
{
    if (!s || !symbol)
        return ERROR_INVALID_ARGUMENT;
    
    if (s->capacity < s->size + 1)
    {
        Error err = string_reserve(s, s->capacity * 2);
        if (err != ERROR_SUCCESS)
            return err;
    }
    
    memcpy((char*)s->data + s->size * s->char_size, symbol, s->char_size);
    s->size++;
    return ERROR_SUCCESS;
}

Error string_equal(String *s1, String *s2, int *result)
{
    if (!s1 || !s2 || s1->char_size != s2->char_size)
    {
        *result = 0;
        return ERROR_INVALID_ARGUMENT;
    }
    
    *result = memcmp(s1->data, s2->data, s1->size * s1->char_size) == 0;
    return ERROR_SUCCESS;
}