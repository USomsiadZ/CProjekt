#include "display.h"
#include "list.h"
#include <stdio.h>

void display_all_posts(Node* head){
    Node* p = get_list_head(head);
    if (p == NULL) {
        printf("Błąd: head jest NULL\n");
        return;
    }
    
    int count = 0;
    
    while (p != NULL) {
        if (p->data != NULL) {
            printf("ID: %d, Autor: %s, Treść: %s, Kategoria: %s, Zgłoszenia: %d, Status: %s\n", p->data->id, p->data->author, p->data->content, p->data->category, p->data->report_count, p->data->status);
            count++;
        }
        p = p->next;
    }
    
    if (count == 0) {
        printf("Brak wpisów w bazie\n");
    } else {
        printf("Wyświetlono %d wpis(ów)\n", count);
    }
}
