#ifndef SEARCH_H
#define SEARCH_H

#include "types.h"
#include <stdbool.h>

int search_by_author(Node* head, const char* author, bool prefix_match);
int search_by_report_count(Node* head, int count);

#endif
