#include "string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Инициализация пустой строки
void string_init(String *s) 
{
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
}

// Освобождение памяти строки
void string_free(String *s) 
{
    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
}

// Изменение ёмкости строки
int string_reserve(String *s, size_t new_capacity) 
{
    if (new_capacity <= s->capacity)
        return 0;
    
    char *new_data = realloc(s->data, new_capacity);
    if (!new_data)
        return -1; // Ошибка выделения памяти
    
    s->data = new_data;
    s->capacity = new_capacity;
    return 0;
}

// Создание строки из C-строки
void string_from_cstr(String *s, const char *cstr) 
{
    // Очистка предыдущего содержимого
    string_free(s);
    s->size = strlen(cstr);
    s->capacity = s->size + 1; // +1 для нулевого символа
    s->data = malloc(s->capacity);
    strcpy(s->data, cstr);
}

// Генерация случайной строки
void generate_random_string(String *s, int min_len, int max_len) 
{
    int len = min_len + rand() % (max_len - min_len + 1);
    string_reserve(s, len + 1);
    
    // Символы для генерации
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    for (int i = 0; i < len; i++)
        s->data[i] = charset[rand() % (sizeof(charset) - 1)]; // -1 для исключения завершающего нуля
    
    s->data[len] = '\0';
    s->size = len;
}

// Конкатенация строк
void string_concat(const String *a, const String *b, String *result) 
{
    // Очистка предыдущего результата
    string_free(result);
    size_t total_size = a->size + b->size;
    
    result->data = malloc(total_size + 1);
    memcpy(result->data, a->data, a->size); // Копирование первой строки
    memcpy(result->data + a->size, b->data, b->size); // Добавление второй строки
    
    result->data[total_size] = '\0';
    result->size = total_size;
    result->capacity = total_size + 1;
}

// Получение подстроки
void string_substring(const String *s, int i, int j, String *result) 
{
    string_free(result);
    
    // Обработка границ индексов
    if (i < 0)
        i = 0;
    if (j >= s->size)
        j = s->size - 1;
    
    // Если начало после конца
    if (i > j)
    { 
        result->data = NULL;
        result->size = 0;
        return;
    }
    
    size_t substring_size = j - i + 1;
    result->data = malloc(substring_size + 1);
    memcpy(result->data, s->data + i, substring_size);
    result->data[substring_size] = '\0';
    result->size = substring_size;
    result->capacity = substring_size + 1;
}

// Поиск подстроки с учетом регистра
void string_find(const String *s, const String *substr, int *result, int case_sensitive) 
{
    // Инициализация результата
    *result = -1; 
    
    if (substr->size == 0 || substr->size > s->size)
        return; // Невозможный случай
    
    // Выбор функции сравнения
    int (*compare)(int) = case_sensitive ? NULL : tolower;
    
    // Перебор возможных позиций
    for (size_t i = 0; i <= s->size - substr->size; i++)
    { 
        int found = 1;

        // Сравнение символов
        for (size_t j = 0; j < substr->size; j++)
        { 
            char a = s->data[i + j];
            char b = substr->data[j];

            // Преобразование к нижнему регистру
            if (!case_sensitive)
            { 
                a = tolower(a);
                b = tolower(b);
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
            return;
        }
    }
}