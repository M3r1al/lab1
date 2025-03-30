#include "string_utils.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

// Функция для обработки конкатенации строк: запрашивает ввод двух строк (или генерирует их случайно),
// выполняет конкатенацию и выводит результат
void handle_concat(String *s1, String *s2, String *result)
{
    // Первый ввод
    printf("Первая строка (Enter - сгенерировать): ");
    string_read(s1);
    
    // Второй ввод (аналогично первому)
    printf("Вторая строка (Enter - сгенерировать): ");
    string_read(s2);
    
    // Совмещаем строки
    string_concat(s1, s2, result);
    string_print("Результат: ", result);
}

// Функция для обработки выделения подстроки: запрашивает исходную строку (или генерирует её),
// начальный и конечный индексы, затем выводит результат
void handle_substring(String *s, String *result)
{
    // Ввод основной строки
    printf("Введите строку (Enter - сгенерировать): ");
    string_read(s);
    
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
    string_print("Подстрока: ", result);
}

// Функция для поиска подстроки в основной строке с возможностью выбора чувствительности к регистру
void handle_find(String *s, String *substr)
{
    // Ввод основной строки
    printf("Основная строка (Enter - сгенерировать): ");
    string_read(s);
    
    // Ввод подстроки
    printf("Подстрока (Enter - сгенерировать): ");
    string_read(substr);
    
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