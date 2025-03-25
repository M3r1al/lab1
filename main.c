#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "string_utils.h"
#include "tests.h"
#include "handlers.h"

int main()
{
    srand(time(NULL));
    
    // Выполнение тестов функций работы со строками
    run_all_tests();

    // Динамический буфер для хранения команды пользователя
    char *cmd_buf = NULL;
    size_t cmd_cap = 0, cmd_size = 0;

    printf("Введите количество байт для символа в строках: ");

    int char_size = 0;

    while (char_size <= 0)
    {
        int ch;
        cmd_size = 0;
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            if (cmd_size >= cmd_cap)
            {
                cmd_cap = cmd_cap ? cmd_cap * 2 : 16;
                cmd_buf = realloc(cmd_buf, cmd_cap);
            }
            cmd_buf[cmd_size++] = ch;
        }
        
        // Прерывание ввода (например, Ctrl+D)
        if (ch == EOF && cmd_size == 0)
            break;
        
        cmd_buf = realloc(cmd_buf, cmd_size + 1);
        cmd_buf[cmd_size] = '\0';
        
        // Проверка корректности ввода команды
        if (cmd_size == 0)
        {
            printf("Ошибка: пустая команда\n");
            continue;
        }
        
        // Проверка, что команда содержит только цифры
        int valid = 1;
        for (char *c = cmd_buf; *c != '\0'; c++)
        {
            if (!isdigit(*c))
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            printf("Ошибка: некорректный ввод команды\n");
            continue;
        }
        
        // Преобразование строки в целое число
        char_size = atoi(cmd_buf);
        if (char_size > 0)
            break;

        printf("Ошибка: количество байт должно быть больше 0\n");
    }
    
    // Инициализация структур для хранения строк
    String s1, s2, result;
    string_init(&s1, char_size);
    string_init(&s2, char_size);
    string_init(&result, char_size);
    
    
    
    while (1)
    {
        printf("\nМЕНЮ:\n");
        printf("1 - Конкатенация\n");
        printf("2 - Подстрока\n");
        printf("3 - Поиск\n");
        printf("4 - Выход\n");
        printf("Выберите действие: ");
        
        // Читаем команду динамически (для поддержки произвольной длины ввода)
        int ch;
        cmd_size = 0;
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            if (cmd_size >= cmd_cap)
            {
                cmd_cap = cmd_cap ? cmd_cap * 2 : 16;
                cmd_buf = realloc(cmd_buf, cmd_cap);
            }
            cmd_buf[cmd_size++] = ch;
        }
        
        // Прерывание ввода (например, Ctrl+D)
        if (ch == EOF && cmd_size == 0)
            break;
        
        cmd_buf = realloc(cmd_buf, cmd_size + 1);
        cmd_buf[cmd_size] = '\0';
        
        // Проверка корректности ввода команды
        if (cmd_size == 0)
        {
            printf("Ошибка: пустая команда\n");
            continue;
        }
        
        // Проверка, что команда содержит только цифры
        int valid = 1;
        for (char *c = cmd_buf; *c != '\0'; c++)
        {
            if (!isdigit(*c))
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            printf("Ошибка: некорректный ввод команды\n");
            continue;
        }
        
        // Преобразование строки в целое число
        int choice = atoi(cmd_buf);
        
        switch (choice) 
        {
            case 1:
                handle_concat(&s1, &s2, &result);
                break;

            case 2:
                handle_substring(&s1, &result);
                break;

            case 3:
                handle_find(&s1, &s2);
                break;

            case 4:
                cleanup_and_exit(&s1, &s2, &result);
                return 0;

            default:
                printf("Ошибка: неверный выбор\n");
        }
    }
    
    // Освобождение динамического буфера команд
    free(cmd_buf);
    // Очистка памяти перед завершением
    cleanup(&s1, &s2, &result);
    return 0;
}