#ifndef HANDLERS_H
#define HANDLERS_H

#include "string_utils.h"

// Обработчики команд меню
void handle_concat(String *s1, String *s2, String *result);
void handle_substring(String *s, String *result);
void handle_find(String *s, String *substr);

// Управление памятью
void cleanup_and_exit(String *s1, String *s2, String *result);
void cleanup(String *s1, String *s2, String *result);

#endif // HANDLERS_H