#include "tests.h"
#include "handlers.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define GREEN(text) "\033[32m" text "\033[0m"
#define RED(text) "\033[31m" text "\033[0m"

#define TEST_CHECK(condition, message) \
    do { \
        if (condition) \
            printf("%s!\n", GREEN("OK")); \
        else \
            printf("%s: %s!\n", RED("Failed"), message); \
    } while(0)

/* Функции вывода для тестов */
void print_concate_test(char *a, char *b, char *expected, char *result)
{
    printf("    Первая строка=\"%s\"\n", a);
    printf("    Вторая строка=\"%s\"\n", b);
    printf("    Ожидается: \"%s\"\n", expected);
    printf("    Получено: \"%s\"\n", result);
}

void print_substring_test(char *source, int start, int end, char *expected, char *result)
{
    printf("    Источник=\"%s\"\n", source);
    printf("    Начало=%d, Конец=%d\n", start, end);
    printf("    Ожидается: \"%s\"\n", expected);
    printf("    Получено: \"%s\"\n", result);
}

void print_find_test(char *source, char *substr, int case_sensitive, int expected, int result)
{
    printf("    Источник=\"%s\"\n", source);
    printf("    Подстрока=\"%s\"\n", substr);
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
    string_concat(&a, &b, &result);
    print_concate_test(a.data, b.data, "Hello World", result.data);
    TEST_CHECK(strcmp(result.data, "Hello World") == 0, "Неверная конкатенация");
    
    // Тест 2: Пустая вторая строка
    string_from_cstr(&b, "");
    printf("  Тест 2:\n");
    string_concat(&a, &b, &result);
    print_concate_test(a.data, b.data, "Hello", result.data);
    TEST_CHECK(strcmp(result.data, "Hello") == 0, "Неверная конкатенация с пустой строкой");
    
    // Тест 3: Обе строки пустые
    string_from_cstr(&a, "");
    string_from_cstr(&b, "");
    printf("  Тест 3:\n");
    string_concat(&a, &b, &result);
    print_concate_test("", "", "", result.data);
    TEST_CHECK(result.size == 0 && result.data != NULL, "Неверная конкатенация пустых строк");
    
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
    string_substring(&s, 1, 4, &result);
    print_substring_test(s.data, 1, 4, "ello", result.data);
    TEST_CHECK(strcmp(result.data, "ello") == 0, "Неверная подстрока");
    
    // Тест 2: Индексы за границами
    printf("  Тест 2:\n");
    string_substring(&s, -5, 20, &result);
    print_substring_test(s.data, -5, 20, "Hello World", result.data);
    TEST_CHECK(strcmp(result.data, "Hello World") == 0, "Неверная обработка границ");
    
    // Тест 3: Пустая подстрока
    printf("  Тест 3:\n");
    string_substring(&s, 3, 2, &result);
    print_substring_test(s.data, 3, 2, "", result.data);
    TEST_CHECK(result.size == 0 && result.data == NULL, "Неверная пустая подстрока");
    
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
    string_find(&s, &substr, &index, 1);
    print_find_test(s.data, substr.data, 1, 6, index);
    TEST_CHECK(index == 6, "Неверный поиск с регистром");
    
    // Тест 2: Поиск без регистра
    string_from_cstr(&substr, "WORLD");
    printf("  Тест 2:\n");
    string_find(&s, &substr, &index, 0);
    print_find_test(s.data, substr.data, 0, 6, index);
    TEST_CHECK(index == 6, "Неверный поиск без регистра");
    
    // Тест 3: Поиск отсутствующей подстроки
    string_from_cstr(&substr, "Test");
    printf("  Тест 3:\n");
    string_find(&s, &substr, &index, 1);
    print_find_test(s.data, substr.data, 1, -1, index);
    TEST_CHECK(index == -1, "Неверный поиск отсутствующей подстроки");
    
    // Тест 4: Поиск в начале
    string_from_cstr(&substr, "Hello");
    printf("  Тест 4:\n");
    string_find(&s, &substr, &index, 1);
    print_find_test(s.data, substr.data, 1, 0, index);
    TEST_CHECK(index == 0, "Неверный поиск в начале");
    
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