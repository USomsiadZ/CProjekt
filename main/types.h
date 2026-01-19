#ifndef TYPES_H
#define TYPES_H

#include "constants.h"

typedef struct Wpis {
    int id;
    char author[MAX_AUTHOR_LEN];
    char content[MAX_CONTENT_LEN];
    char category[MAX_CATEGORY_LEN];
    int report_count;
    char status[MAX_STATUS_LEN];
} Wpis;

typedef struct Node {
    Wpis* data;
    struct Node* prev;
    struct Node* next;
} Node;

#endif
