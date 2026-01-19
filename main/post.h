#ifndef POST_H
#define POST_H

#include "types.h"
#include <stdbool.h>

int get_next_id(Node* head);
Wpis* add_post(Node* head, const char* author, const char* content, const char* category, const char* status);
Wpis* find_post_by_id(Node* head, int id);
bool can_delete_post(const Wpis* wpis);
int remove_post(Node* head, int id);
int remove_posts_by_status(Node* head, const char* status);
int update_post_status(Node* head, int id, const char* new_status);
int edit_post(Node* head, int id, int field, const void* new_value);

#endif
