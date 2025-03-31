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

char* print_change_error(String a, String b)
{
    printf("    Было: %s\n    Стало: %s\n", (char*)a.data, (char*)b.data);
    return "Данные изменились в ходе работы программы:";
}

void test_concat(char *a_cstr, char *b_cstr, char *result_cstr)
{
    Error err;
    String a, b, result, a_copy, b_copy;
    TypeInfo* type = create_char_type_info();

    err = string_init(&a, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_init(&b, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    // Копии
    err = string_init(&a_copy, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_init(&b_copy, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_init(&result, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    err = string_from_cstr(&a, a_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_from_cstr(&b, b_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    // Копии
    err = string_from_cstr(&a_copy, a_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_from_cstr(&b_copy, b_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_concat(&a, &b, &result);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    // Проверка копий
    int equal;
    err = string_equal(&a_copy, &a, &equal);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    if (!equal)
    {
        TEST_CHECK(0, print_change_error(a_copy, a));
        return;
    }

    err = string_equal(&b_copy, &b, &equal);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    if (!equal)
    {
        TEST_CHECK(0, print_change_error(b_copy, b));
        return;
    }

    print_concate_test(a.data, b.data, result_cstr, result.data);
    TEST_CHECK(strcmp((char*)result.data, result_cstr) == 0, "Неверная конкатенация");
    
    // Освобождение памяти
    string_free(&a);
    string_free(&b);
    string_free(&result);
    string_free(&a_copy);
    string_free(&b_copy);
}

// Тесты на конкатенацию
void run_test_concat() 
{
    printf("Тестирование конкатенации:\n");
    
    // Тест 1: Обычный случай
    printf("  Тест 1:\n");
    test_concat("Hello", " World", "Hello World");
    
    // Тест 2: Пустая вторая строка
    printf("  Тест 2:\n");
    test_concat("Hello", "", "Hello");
    
    // Тест 3: Обе строки пустые
    printf("  Тест 3:\n");
    test_concat("", "", "");
}

void test_substring(char *a_cstr, int i, int j, char *result_cstr)
{
    Error err;
    String a, result, a_copy;
    TypeInfo* type = create_char_type_info();
    
    err = string_init(&a, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    err = string_init(&result, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    // Копия
    err = string_init(&a_copy, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_from_cstr(&a, a_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    err = string_from_cstr(&result, result_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    // Копия
    err = string_from_cstr(&a_copy, a_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    err = string_substring(&a, i, j, &result);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    // Проверка копий
    int equal;
    err = string_equal(&a_copy, &a, &equal);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    if (!equal)
    {
        TEST_CHECK(0, print_change_error(a_copy, a));
        return;
    }
    
    print_substring_test(a.data, i, j, result_cstr, result.data);
    TEST_CHECK(strcmp((char*)result.data, result_cstr) == 0, "Неверная подстрока");

    // Освобождение памяти
    string_free(&a);
    string_free(&result);
    string_free(&a_copy);
}

// Тесты на подстроку
void run_test_substring() 
{
    printf("\nТестирование подстроки:\n");
    
    // Тест 1: Нормальный случай
    printf("  Тест 1:\n");
    test_substring("Hello World", 1, 4, "ello");
    
    // Тест 2: Индексы на границах
    printf("  Тест 2:\n");
    test_substring("Hello World", 0, 10, "Hello World");
}

void test_find(char *a_cstr, char *sub_cstr, int case_sensitive, int result, char *error_message)
{
    Error err;
    String a, substr, a_copy, sub_copy;
    int index;
    TypeInfo* type = create_char_type_info();
    
    err = string_init(&a, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    err = string_init(&substr, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    // Копии
    err = string_init(&a_copy, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    err = string_init(&sub_copy, type);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_from_cstr(&a, a_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_from_cstr(&substr, sub_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    // Копия
    err = string_from_cstr(&a_copy, a_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    err = string_from_cstr(&sub_copy, sub_cstr);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    
    err = string_find(&a, &substr, &index, case_sensitive);
    if (err != ERROR_SUCCESS && err != ERROR_INDEX_OUT_OF_RANGE)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }

    // Проверка копий
    int equal;
    err = string_equal(&a_copy, &a, &equal);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    if (!equal)
    {
        TEST_CHECK(0, print_change_error(a_copy, a));
        return;
    }

    err = string_equal(&substr, &sub_copy, &equal);
    if (err != ERROR_SUCCESS)
    {
        TEST_CHECK(0, cstr_error(err));
        return;
    }
    if (!equal)
    {
        TEST_CHECK(0, print_change_error(sub_copy, substr));
        return;
    }

    print_find_test(a.data, substr.data, case_sensitive, result, index);

    if (!error_message)
        error_message = "Неверный поиск подстроки";
    
    TEST_CHECK(index == result, error_message);

    // Освобождение памяти
    string_free(&a);
    string_free(&substr);
    string_free(&a_copy);
    string_free(&sub_copy);
}

// Тесты на поиск подстроки
void run_test_find() 
{
    printf("\nТестирование поиска:\n");
    
    // Тест 1: Поиск с регистром
    printf("  Тест 1:\n");
    test_find("Hello World", "World", 1, 6, "Неверный поиск с регистром");
    
    // Тест 2: Поиск без регистра
    printf("  Тест 2:\n");
    test_find("Hello World", "WORLD", 0, 6, "Неверный поиск с регистром");
    
    // Тест 3: Поиск отсутствующей подстроки
    printf("  Тест 3:\n");
    test_find("Hello World", "Test", 1, -1, "Неверный поиск отсутствующей подстроки");
    
    // Тест 4: Поиск в начале
    printf("  Тест 4:\n");
    test_find("Hello World", "Hello", 1, 0, "Неверный поиск в начале");
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