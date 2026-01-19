#include "list.h"
#include <stdlib.h>
#include <stdio.h>

Node* get_list_head(Node* head) {
    if (head == NULL) {
        return NULL;
    }
    Node* p = head;
    while (p->prev != NULL) {
        p = p->prev;
    }
    if (p->data == NULL) {
        return NULL;
    }
    return p;
}

Node* create_node(void){
    Node* n = calloc(1, sizeof(Node));
    if(n == NULL){
        printf("Błąd alokacji pamięci\n");
        return NULL;
    }
    n->data = NULL;
    n->prev = NULL;
    n->next = NULL;
    return n;
}

void destroy_node(Node* head) {
    if (head == NULL) {
        printf("Błąd: head jest NULL\n");
        return;
    }
    // znajdź początek listy
    while (head->prev != NULL) {
        head = head->prev;
    }
    // usuń wszystkie węzły
    Node* p = head;
    while (p != NULL) {
        Node* next = p->next;
        if (p->data != NULL) {
            free(p->data);
        }
        free(p);
        p = next;
    }
}
