#include "search.h"
#include "list.h"
#include <stdio.h>
#include <string.h>

int search_by_author(Node* head, const char* author, bool prefix_match) {
    if (author == NULL) {
        printf("Błąd: nieprawidłowe parametry\n");
        return 0;
    }
    
    Node* p = get_list_head(head);
    if (p == NULL) {
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    int count = 0;
    size_t author_len = strlen(author);
    
    while (p != NULL) {
        bool match = false;
        if (prefix_match) {
            match = (strncmp(p->data->author, author, author_len) == 0);
        } else {
            match = (strcmp(p->data->author, author) == 0);
        }
        
        if (match) {
            printf("ID: %d, Autor: %s, Treść: %s, Kategoria: %s, Zgłoszenia: %d, Status: %s\n", p->data->id, p->data->author, p->data->content, p->data->category, p->data->report_count, p->data->status);
            count++;
        }
        p = p->next;
    }
    
    if (count == 0) {
        printf("Nie znaleziono wpisów dla autora: %s\n", author);
    } else {
        printf("Znaleziono %d wpis(ów)\n", count);
    }
    
    return count;
}

int search_by_report_count(Node* head, int count) {
    if (count < 0) {
        printf("Błąd: nieprawidłowe parametry\n");
        return 0;
    }
    
    Node* p = get_list_head(head);
    if (p == NULL) {
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    int found = 0;
    
    while (p != NULL) {
        if (p->data->report_count == count) {
            printf("ID: %d, Autor: %s, Treść: %s, Kategoria: %s, Zgłoszenia: %d, Status: %s\n", p->data->id, p->data->author, p->data->content, p->data->category, p->data->report_count, p->data->status);
            found++;
        }
        p = p->next;
    }
    
    if (found == 0) {
        printf("Nie znaleziono wpisów z liczbą zgłoszeń: %d\n", count);
    } else {
        printf("Znaleziono %d wpis(ów)\n", found);
    }
    
    return found;
}
