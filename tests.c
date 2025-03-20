#include "tests.h"
#include "handlers.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>

#define GREEN(text) "\033[32m" text "\033[0m"
#define RED(text) "\033[31m" text "\033[0m"

#define TEST_CHECK(condition, message) \
    do { \
        if (condition) \
            printf("%s!\n", GREEN("OK")); \
        else \
            printf("%s: %s!\n", RED("Failed"), message); \
    } while(0)

void print_concate_test(wchar_t *a, wchar_t *b, char *expected, wchar_t *result)
{
    printf("    Первая строка=\"%ls\"\n", a);
    printf("    Вторая строка=\"%ls\"\n", b);
    printf("    Ожидается: \"%s\"\n", expected);
    printf("    Получено: \"%ls\"\n", result);
}

void print_substring_test(wchar_t *source, int start, int end, char *expected, wchar_t *result)
{
    printf("    Источник=\"%ls\"\n", source);
    printf("    Начало=%d, Конец=%d\n", start, end);
    printf("    Ожидается: \"%s\"\n", expected);
    printf("    Получено: \"%ls\"\n", result);
}

void print_find_test(wchar_t *source, wchar_t *substr, int case_sensitive, int expected, int result)
{
    printf("    Источник=\"%ls\"\n", source);
    printf("    Подстрока=\"%ls\"\n", substr);
    printf("    Регистр: %s\n", case_sensitive ? "Да" : "Нет");
    printf("    Ожидается: %d\n", expected);
    printf("    Получено: %d\n", result);
}

// Тесты на конкатенацию
void run_test_concat() 
{
    printf("Тестирование конкатенации:\n");
    String a, b, result;
    string_init(&a);
    string_init(&b);
    string_init(&result);
    
    // Тест 1: Обычный случай
    string_from_cstr(&a, "Hello");
    string_from_cstr(&b, " World");
    printf("  Тест 1:\n");
    Error err = string_concat(&a, &b, &result);
    print_concate_test(a.data, b.data, "Hello World", result.data);
    TEST_CHECK(wcscmp(result.data, L"Hello World") == 0 && err == ERROR_SUCCESS, "Неверная конкатенация");
    
    // Тест 2: Пустая вторая строка
    string_from_cstr(&b, "");
    printf("  Тест 2:\n");
    err = string_concat(&a, &b, &result);
    print_concate_test(a.data, b.data, "Hello", result.data);
    TEST_CHECK(wcscmp(result.data, L"Hello") == 0 && err == ERROR_SUCCESS, "Неверная конкатенация с пустой строкой");
    
    // Тест 3: Обе строки пустые
    string_from_cstr(&a, "");
    string_from_cstr(&b, "");
    printf("  Тест 3:\n");
    err = string_concat(&a, &b, &result);
    print_concate_test(a.data, b.data, "", result.data);
    TEST_CHECK(result.size == 0 && result.data != NULL && err == ERROR_SUCCESS, "Неверная конкатенация пустых строк");
    
    string_free(&a);
    string_free(&b);
    string_free(&result);
}

// Тесты на подстроку
void run_test_substring() 
{
    printf("\nТестирование подстроки:\n");
    String s, result;
    string_init(&s);
    string_init(&result);
    
    // Тест 1: Нормальный случай
    string_from_cstr(&s, "Hello World");
    printf("  Тест 1:\n");
    Error err = string_substring(&s, 1, 4, &result);
    print_substring_test(s.data, 1, 4, "ello", result.data);
    TEST_CHECK(wcscmp(result.data, L"ello") == 0 && err == ERROR_SUCCESS, "Неверная подстрока");
    
    // Тест 2: Индексы на границах
    printf("  Тест 2:\n");
    err = string_substring(&s, 0, 10, &result);
    print_substring_test(s.data, 0, 10, "Hello World", result.data);
    TEST_CHECK(wcscmp(result.data, L"Hello World") == 0 && err == ERROR_SUCCESS, "Неверная обработка границ");
    
    string_free(&s);
    string_free(&result);
}

// Тесты на поиск подстроки
void run_test_find() 
{
    printf("\nТестирование поиска:\n");
    String s, substr;
    int index;
    string_init(&s);
    string_init(&substr);
    
    // Тест 1: Поиск с регистром
    string_from_cstr(&s, "Hello World");
    string_from_cstr(&substr, "World");
    printf("  Тест 1:\n");
    Error err = string_find(&s, &substr, &index, 1);
    print_find_test(s.data, substr.data, 1, 6, index);
    TEST_CHECK(index == 6 && (err == ERROR_SUCCESS || err == ERROR_INDEX_OUT_OF_RANGE), "Неверный поиск с регистром");
    
    // Тест 2: Поиск без регистра
    string_from_cstr(&substr, "WORLD");
    printf("  Тест 2:\n");
    err = string_find(&s, &substr, &index, 0);
    print_find_test(s.data, substr.data, 0, 6, index);
    TEST_CHECK(index == 6 && (err == ERROR_SUCCESS || err == ERROR_INDEX_OUT_OF_RANGE), "Неверный поиск без регистра");
    
    // Тест 3: Поиск отсутствующей подстроки
    string_from_cstr(&substr, "Test");
    printf("  Тест 3:\n");
    err = string_find(&s, &substr, &index, 1);
    print_find_test(s.data, substr.data, 1, -1, index);
    TEST_CHECK(index == -1 && (err == ERROR_SUCCESS || err == ERROR_INDEX_OUT_OF_RANGE), "Неверный поиск отсутствующей подстроки");
    
    // Тест 4: Поиск в начале
    string_from_cstr(&substr, "Hello");
    printf("  Тест 4:\n");
    err = string_find(&s, &substr, &index, 1);
    print_find_test(s.data, substr.data, 1, 0, index);
    TEST_CHECK(index == 0 && (err == ERROR_SUCCESS || err == ERROR_INDEX_OUT_OF_RANGE), "Неверный поиск в начале");
    
    string_free(&s);
    string_free(&substr);
}

// Запуск всех тестов
void run_all_tests() 
{
    printf("========================================\n");
    printf("Запуск тестирования\n");
    printf("========================================\n");
    
    run_test_concat();
    run_test_substring();
    run_test_find();
    
    printf("========================================\n");
    printf("Тестирование завершено\n");
    printf("========================================\n\n");
}