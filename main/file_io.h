#ifndef FILE_IO_H
#define FILE_IO_H

#include "types.h"

int save_to_file(Node* head, const char* filename);
int save_to_file_bin(Node* head, const char* filename);
int load_from_file(Node* head, const char* filename);
int load_from_file_bin(Node* head, const char* filename);

#endif
