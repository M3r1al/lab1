#include "string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>

// Инициализация пустой строки
Error string_init(String *s) 
{
    if (!s)
        return ERROR_INVALID_ARGUMENT;
    
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
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
    
    wchar_t *new_data = (wchar_t*)realloc(s->data, new_capacity * sizeof(wchar_t));
    if (!new_data)
        return ERROR_OUT_OF_MEMORY; // Ошибка выделения памяти
    
    s->data = new_data;
    s->capacity = new_capacity;
    return ERROR_SUCCESS;
}

// Создание строки из C-строки
Error string_from_cstr(String *s, const char *cstr) 
{
    if (!s)
        return ERROR_INVALID_ARGUMENT;
    
    // Очистка предыдущего содержимого
    Error err = string_free(s);
    if (err != ERROR_SUCCESS)
        return err;
    size_t len = strlen(cstr) + 1;
    err = string_reserve(s, len);
    if (err != ERROR_SUCCESS)
        return err;
    
    for (size_t i = 0; i < len; i++)
        s->data[i] = (wchar_t)cstr[i];
    
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
        s->data[i] = (wchar_t)charset[rand() % (sizeof(charset) - 1)]; // -1 для исключения завершающего нуля
    
    s->data[len] = L'\0';
    s->size = len;
    return ERROR_SUCCESS;
}

// Конкатенация строк
Error string_concat(const String *a, const String *b, String *result) 
{
    if (!a || !b)
        return ERROR_INVALID_ARGUMENT;
    
    // Очистка предыдущего результата
    Error err = string_free(result);
    if (err != ERROR_SUCCESS)
        return err;
    
    size_t total_size = a->size + b->size;
    err = string_reserve(result, total_size + 1);
    if (err != ERROR_SUCCESS)
        return err;
    
    for (size_t i = 0; i < a->size; i++)
        result->data[i] = a->data[i];
    
    for (size_t i = 0; i < b->size; i++)
        result->data[a->size + i] = b->data[i];
    
    result->data[total_size] = L'\0';
    result->size = total_size;
    return ERROR_SUCCESS;
}

// Получение подстроки
Error string_substring(const String *s, int i, int j, String *result) 
{
    if (!s || i > j)
        return ERROR_INVALID_ARGUMENT;
    
    // Обработка границ индексов
    if (wcslen(s->data) < j || i < 0)
        return ERROR_INDEX_OUT_OF_RANGE;
    
    Error err = string_free(result);
    if (err != ERROR_SUCCESS)
        return err;
    
    size_t substring_size = j - i + 1;
    err = string_reserve(result, substring_size + 1);
    if (err != ERROR_SUCCESS)
        return err;
    
    for (size_t k = 0; k < substring_size; k++)
        result->data[k] = s->data[i + k];
        
    result->data[substring_size] = L'\0';
    result->size = substring_size;
    return ERROR_SUCCESS;
}

// Поиск подстроки с учетом регистра
Error string_find(const String *s, const String *substr, int *result, int case_sensitive) 
{
    if (!s || !substr || !result)
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
            wchar_t a = s->data[i + j];
            wchar_t b = substr->data[j];

            // Преобразование к нижнему регистру
            if (!case_sensitive)
            { 
                a = towlower(a);
                b = towlower(b);
            }
            
            if (a != b)
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

Error string_get_symbol(const String *s, size_t index, wchar_t *symbol)
{
    if (!s || !symbol)
        return ERROR_INVALID_ARGUMENT;
    if (index >= s->size)
        return ERROR_INDEX_OUT_OF_RANGE;
    
    *symbol = s->data[index];
    return ERROR_SUCCESS;
}

Error string_set_symbol(String *s, size_t index, wchar_t symbol)
{
    if (!s)
        return ERROR_INVALID_ARGUMENT;
    if (index >= s->size)
        return ERROR_INDEX_OUT_OF_RANGE;
    
    s->data[index] = symbol;
    return ERROR_SUCCESS;
}

Error string_append_symbol(String *s, wchar_t symbol)
{
    if (!s)
        return ERROR_INVALID_ARGUMENT;
    
    Error err = string_reserve(s, s->capacity * 2 + 1);
    if (err != ERROR_SUCCESS)
        return err;
    
    s->data[s->size] = symbol;
    s->size++;
    s->data[s->size] = L'\0';
    return ERROR_SUCCESS;
}