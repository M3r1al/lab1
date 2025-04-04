#include "string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>

char* cstr_error(Error err)
{
    switch (err)
    {
        case ERROR_SUCCESS:
            return "Нет ошибок";
        case ERROR_INVALID_ARGUMENT:
            return "Неверные аргументы";
        case ERROR_OUT_OF_MEMORY:
            return "Недостаточно памяти";
        case ERROR_INDEX_OUT_OF_RANGE:
            return "Индекс выходит за границы";
        default:
            return "Неизвестная ошибка";
    }
}

void string_read(String *s)
{
    // Получаем TypeInfo из структуры String
    const TypeInfo *type = s->type;
    size_t char_size = type->char_size;
    void *input = NULL;
    size_t capacity = 0, size = 0;
    
    int ch;
    size = 0;

    // Считываем символы до конца строки или EOF
    while ((ch = getchar()) != '\n' && ch != EOF) 
    {
        // Если буфер переполнен, увеличиваем его размер
        if (size >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            input = realloc(input, capacity * char_size);
        }
        // Преобразуем char в символ типа TypeInfo
        size_t offset = size * char_size;
        type->char_to_symbol(type, (char*)input + offset, (void*)&ch);
        size++;
    }

    // Если ввод был прерван без ввода данных
    if (ch == EOF && size == 0) 
    {
        free(input);
        return;
    }

    // Добавляем нулевой символ для завершения строки
    input = realloc(input, (size + 1) * char_size); // Учтено char_size
    size_t null_offset = size * char_size;
    char c = '\0';
    type->char_to_symbol(type, (char*)input + null_offset, (void*)&c); // Учтено '\0'

    // Если строка пустая - генерируем случайную
    if (size == 0) 
    {
        generate_random_string(s, 5, 15);
        string_print("  Сгенерирована: ", s);
    }
    else 
    {
        // Инициализируем строку, используя TypeInfo
        s->size = size;
        s->capacity = capacity; // Или выделяем больше, если нужно
        s->data = malloc(size * char_size); // Выделяем память под символы

        // Копируем данные из временного буфера input в s->data
        for (size_t i = 0; i < size; i++) 
        {
            size_t src_offset = i * char_size;
            size_t dst_offset = i * char_size;
            type->copy((char*)s->data + dst_offset, (char*)input + src_offset);
        }
    }

    free(input); // Освобождаем временный буфер
}

void string_print(const char *message, String *s)
{
    if (s->type->char_size == 1)
        printf("%s%s\n", message, s->data);
    else if (s->type->char_size == sizeof(wchar_t))
        printf("%s%ls\n", message, s->data);
    else
    {
        printf("%s",message);
        const unsigned char *data = s->data;
        
        for (size_t i = 0; i < s->size * s->type->char_size;)
        {
            // Выводим все байты текущего символа
            for (size_t j = 0; j < s->type->char_size; j++)
                printf("%hhX", data[i + j]);
            
            printf(" "); // Разделитель между символами
            i += s->type->char_size;
        }
        printf("\n");
    }
}

// Инициализация пустой строки
Error string_init(String *s, TypeInfo *type) 
{
    if (!s || !type)
        return ERROR_INVALID_ARGUMENT;
    
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
    s->type = type;
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
Error string_reserve(String *s, int new_capacity) 
{
    if (!s || new_capacity <= 0)
        return ERROR_INVALID_ARGUMENT;
    
    if (new_capacity <= s->capacity)
        return ERROR_SUCCESS;
    
    void *new_data = realloc(s->data, new_capacity * s->type->char_size);
    if (!new_data)
        return ERROR_OUT_OF_MEMORY; // Ошибка выделения памяти
    
    s->data = new_data;
    s->capacity = new_capacity;
    return ERROR_SUCCESS;
}

// Создание строки из C-строки
Error string_from_cstr(String *s, const char *cstr) 
{
    if (!s || !cstr || s->type->char_size != 1)
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
        memcpy((char*)s->data + (i * s->type->char_size), &c, 1);
    }
    
    memset((char*)s->data + (len * s->type->char_size), 0, s->type->char_size);
    s->size = len;
    return ERROR_SUCCESS;
}

// Конкатенация строк
Error string_concat(const String *a, const String *b, String *result) 
{
    if (!a || !b || a->type->char_size != b->type->char_size || b->type->char_size != result->type->char_size)
        return ERROR_INVALID_ARGUMENT;
    
    // Очистка предыдущего результата
    Error err = string_free(result);
    if (err != ERROR_SUCCESS)
        return err;
    
    size_t total_size = a->size + b->size;
    err = string_reserve(result, total_size + 1);
    if (err != ERROR_SUCCESS)
        return err;
    
    memcpy(result->data, a->data, a->size * a->type->char_size);
    memcpy((char*)result->data + (a->size * a->type->char_size), b->data, b->size * b->type->char_size);
    result->size = total_size;
    return ERROR_SUCCESS;
}

// Получение подстроки
Error string_substring(const String *s, int i, int j, String *result) 
{
    if (!s || i > j || s->type->char_size != result->type->char_size)
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
        size_t src_offset = (i + k) * s->type->char_size;
        size_t dst_offset = k * result->type->char_size;
        memcpy((char*)result->data + dst_offset, (char*)s->data + src_offset, s->type->char_size);
    }
        
    memset((char*)result->data + substring_size * result->type->char_size, 0, result->type->char_size);
    result->size = substring_size;
    return ERROR_SUCCESS;
}

// Поиск подстроки с учетом регистра
Error string_find(const String *s, const String *substr, int *result, int case_sensitive) 
{
    if (!s || !substr || !result || s->type->char_size != substr->type->char_size)
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
            size_t s_offset = (i + j) * s->type->char_size;
            size_t substr_offset = j * substr->type->char_size;

            // Временные копии для преобразования регистра
            void *a = malloc(s->type->char_size);
            void *b = malloc(substr->type->char_size);
            memcpy(a, (char*)s->data + s_offset, s->type->char_size);
            memcpy(b, (char*)substr->data + substr_offset, substr->type->char_size);

            // Преобразование к нижнему регистру
            if (!case_sensitive)
            { 
                // char
                if (s->type->char_size == 1)
                {
                    *(char*)a = towlower(*(char*)a);
                    *(char*)b = towlower(*(char*)b);
                }
                // wchar_t
                if (s->type->char_size == sizeof(wchar_t))
                {
                    *(wchar_t*)a = towlower(*(wchar_t*)a);
                    *(wchar_t*)b = towlower(*(wchar_t*)b);
                }
            }
            
            if (memcmp(a, b, s->type->char_size) != 0)
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

Error string_get_symbol(const String *s, int index, void *symbol)
{
    if (!s || !symbol)
        return ERROR_INVALID_ARGUMENT;
    if (index >= s->size || index < 0)
        return ERROR_INDEX_OUT_OF_RANGE; // Такой же вопрос про тип ошибки
    
    memcpy(symbol, (char*)s->data + (index * s->type->char_size), s->type->char_size);
    return ERROR_SUCCESS;
}

Error string_set_symbol(String *s, int index, const void *symbol)
{
    if (!s || !symbol)
        return ERROR_INVALID_ARGUMENT;
    if (index >= s->size || index < 0)
        return ERROR_INDEX_OUT_OF_RANGE; // Аналогично, это к аргументу или ошибке с индексом относится?
    
    memcpy((char*)s->data + (index * s->type->char_size), symbol, s->type->char_size);
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
    
    memcpy((char*)s->data + s->size * s->type->char_size, symbol, s->type->char_size);
    s->size++;
    return ERROR_SUCCESS;
}

Error string_equal(String *s1, String *s2, int *result)
{
    if (!s1 || !s2 || s1->type != s2->type)
    {
        *result = 0;
        return ERROR_INVALID_ARGUMENT;
    }
    
    *result = memcmp(s1->data, s2->data, s1->size * s1->type->char_size) == 0;
    return ERROR_SUCCESS;
}