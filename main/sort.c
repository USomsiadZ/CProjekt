#include "sort.h"
#include "list.h"
#include <stdio.h>
#include <string.h>

// Pomocnicza funkcja zamiany wpisów
static void swap_wpis(Wpis** a, Wpis** b) {
    Wpis* temp = *a;
    *a = *b;
    *b = temp;
}

Node* sort_by_author(Node* head){
    if (head == NULL) {
        printf("Błąd: head jest NULL\n");
        return NULL;
    }
    
    Node* p = get_list_head(head);
    
    if (p->data == NULL) {
        return p;
    }
    
    int count = 0;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    p = get_list_head(head);
    
    for (int i = 0; i < count - 1; i++) {
        while (p->next != NULL) {
            if (strcmp(p->data->author, p->next->data->author) > 0) {
                swap_wpis(&p->data, &p->next->data);
            }
            p = p->next;
        }
        while (p->prev != NULL) {
            p = p->prev;
        }
    }
    return p;
}

Node* sort_by_report_count(Node* head){
    if (head == NULL) {
        printf("Błąd: head jest NULL\n");
        return NULL;
    }
    
    Node* p = get_list_head(head);
    
    if (p->data == NULL) {
        return p;
    }
    
    int count = 0;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    p = get_list_head(head);
    
    for (int i = 0; i < count - 1; i++) {
        while (p->next != NULL) {
            if (p->data->report_count > p->next->data->report_count) {
                swap_wpis(&p->data, &p->next->data);
            }
            p = p->next;
        }
        while (p->prev != NULL) {
            p = p->prev;
        }
    }
    return p;
}
