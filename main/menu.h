#ifndef MENU_H
#define MENU_H

#include "types.h"

void read_string(char* buf, int size);
int read_int(void);
void get_category(char* category);
void get_status(char* status);

void show_menu(void);
void show_add_post_menu(Node* head);
void show_edit_post_menu(Node* head);
void show_delete_post_menu(Node* head);
void show_search_menu(Node* head);
Node* show_sort_menu(Node* head);
void show_file_menu(Node* head, const char* filename);
void show_file_menu_bin(Node* head, const char* filename);
Node* option_menu(Node* head, const char* filename);

#endif
