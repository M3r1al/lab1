#include "string_utils.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Функция для обработки конкатенации строк: запрашивает ввод двух строк (или генерирует их случайно),
// выполняет конкатенацию и выводит результат
void handle_concat(String *s1, String *s2, String *result)
{
    char *input = NULL;
    size_t capacity = 0, size = 0;
    
    // Первый ввод
    printf("Первая строка (Enter - сгенерировать): ");
    int ch;
    size = 0;
    // Считываем символы до конца строки или EOF
    while ((ch = getchar()) != '\n' && ch != EOF) 
    {
        // Если буфер переполнен, увеличиваем его размер
        if (size >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            input = realloc(input, capacity);
        }
        input[size++] = ch;
    }
    // Если ввод был прерван без ввода данных
    if (ch == EOF && size == 0) 
    {
        free(input);
        return;
    }
    // Добавляем нулевой символ для завершения строки
    input = realloc(input, size + 1);
    input[size] = '\0';
    
    // Если строка пустая - генерируем случайную
    if (strlen(input) == 0)
    {
        generate_random_string(s1, 5, 15); 
        printf("  Сгенерирована: %s\n", s1->data);
    }
    else
        string_from_cstr(s1, input); // Иначе преобразуем в строку структуры
    
    free(input); // Освобождаем временный буфер
    input = NULL;
    
    // Второй ввод (аналогично первому)
    printf("Вторая строка (Enter - сгенерировать): ");
    size = 0;
    capacity = 0;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (size >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            input = realloc(input, capacity);
        }
        input[size++] = ch;
    }
    if (ch == EOF && size == 0)
    {
        free(input);
        return;
    }
    input = realloc(input, size + 1);
    input[size] = '\0';
    
    if (strlen(input) == 0)
    {
        generate_random_string(s2, 5, 15);
        printf("  Сгенерирована: %s\n", s2->data);
    }
    else
        string_from_cstr(s2, input);
    
    free(input);
    
    // Совмещаем строки
    string_concat(s1, s2, result);
    printf("РЕЗУЛЬТАТ: %s\n", result->data);
}

// Функция для обработки выделения подстроки: запрашивает исходную строку (или генерирует её),
// начальный и конечный индексы, затем выводит результат
void handle_substring(String *s, String *result)
{
    char *input = NULL;
    size_t capacity = 0, size = 0;
    
    // Ввод основной строки
    printf("Введите строку (Enter - сгенерировать): ");
    int ch;
    size = 0;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (size >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            input = realloc(input, capacity);
        }
        input[size++] = ch;
    }
    if (ch == EOF && size == 0)
    {
        free(input);
        return;
    }
    input = realloc(input, size + 1);
    input[size] = '\0';
    
    if (strlen(input) == 0)
    {
        generate_random_string(s, 5, 15);
        printf("  Сгенерирована: %s\n", s->data);
    }
    else
        string_from_cstr(s, input);
    
    free(input);
    
    int i, j;
    printf("Начальный индекс: ");
    // Проверка корректности ввода целого числа
    if (scanf("%d", &i) != 1)
    {
        printf("Ошибка ввода\n");
        while (getchar() != '\n'); // Очистка буфера ввода
        return;
    }

    printf("Конечный индекс: ");
    if (scanf("%d", &j) != 1)
    {
        printf("Ошибка ввода\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n'); // Удаление остатков ввода
    
    // Извлекаем подстроку
    string_substring(s, i, j, result);
    printf("Подстрока: %s\n", result->data);
}

// Функция для поиска подстроки в основной строке с возможностью выбора чувствительности к регистру
void handle_find(String *s, String *substr)
{
    char *input = NULL;
    size_t capacity = 0, size = 0;
    
    // Ввод основной строки
    printf("Основная строка (Enter - сгенерировать): ");
    int ch;
    size = 0;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (size >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            input = realloc(input, capacity);
        }
        input[size++] = ch;
    }
    if (ch == EOF && size == 0)
    {
        free(input);
        return;
    }
    input = realloc(input, size + 1);
    input[size] = '\0';
    
    if (strlen(input) == 0)
    {
        generate_random_string(s, 5, 15);
        printf("  Сгенерирована: %s\n", s->data);
    }
    else
        string_from_cstr(s, input);
    
    free(input);
    
    // Ввод подстроки
    printf("Подстрока (Enter - сгенерировать): ");
    input = NULL;
    capacity = 0;
    size = 0;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (size >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            input = realloc(input, capacity);
        }
        input[size++] = ch;
    }
    if (ch == EOF && size == 0)
    {
        free(input);
        return;
    }
    input = realloc(input, size + 1);
    input[size] = '\0';
    
    if (strlen(input) == 0)
    {
        generate_random_string(substr, 3, 10);
        printf("  Сгенерирована: %s\n", substr->data);
    }
    else
        string_from_cstr(substr, input);
    
    free(input);
    
    int case_sensitive;
    printf("Чувствительность к регистру (1/0): ");
    // Проверка ввода флага
    if (scanf("%d", &case_sensitive) != 1)
    {
        printf("Ошибка ввода\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n'); // Очистка остатков ввода
    
    int index;
    // Выполняем поиск
    string_find(s, substr, &index, case_sensitive); 
    if (index != -1)
        printf("Найдено на позиции: %d\n", index);
    else
        printf("Не найдено\n");
}

// Функция для очистки памяти и завершения программы
void cleanup_and_exit(String *s1, String *s2, String *result)
{
    string_free(s1);
    string_free(s2);
    string_free(result);
    printf("Память освобождена. Выход...\n");
}

// Функция для очистки памяти (без сообщения о выходе)
void cleanup(String *s1, String *s2, String *result)
{
    string_free(s1);
    string_free(s2);
    string_free(result);
}