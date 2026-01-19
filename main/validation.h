#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>

bool validate_author(const char* author);
bool validate_content(const char* content);
bool validate_category(const char* category);
bool validate_status(const char* status);

#endif
