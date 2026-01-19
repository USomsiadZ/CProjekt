#include "post.h"
#include "list.h"
#include "validation.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_next_id(Node* head) {
    Node* p = get_list_head(head);
    
    if (p == NULL) {
        return 1;
    }
    
    int max_id = 0;
    while (p != NULL) {
        if (p->data != NULL) {
            max_id = p->data->id > max_id ? p->data->id : max_id;
        }
        p = p->next;
    }
    
    return max_id + 1;
}

Wpis* add_post(Node* head, const char* author, const char* content, const char* category, const char* status){
    if(!validate_author(author)){
        printf("Błąd: nieprawidłowa nazwa autora\n");
        return NULL;
    }
    if(!validate_content(content)){
        printf("Błąd: nieprawidłowa treść\n");
        return NULL;
    }
    if(!validate_category(category)){
        printf("Błąd: nieprawidłowa kategoria\n");
        return NULL;
    }
    if(!validate_status(status)){
        printf("Błąd: nieprawidłowy status\n");
        return NULL;
    }
    
    Wpis* wpis = malloc(sizeof(Wpis));
    if(wpis == NULL){
        printf("Błąd alokacji pamięci dla wpisu\n");
        return NULL;
    }
    
    // Strcpy jest bezpieczne bo znamy rozmiar danych, które są zawsze zakończone '\0'
    wpis->id = get_next_id(head);
    strcpy(wpis->author, author);
    strcpy(wpis->content, content);
    strcpy(wpis->category, category);
    wpis->report_count = 1;
    strcpy(wpis->status, status);
    

    if (head == NULL) {
        printf("Błąd: head jest NULL\n");
        free(wpis);
        return NULL;
    }
    
    Node* p = head;
    while (p->prev != NULL) {
        p = p->prev;
    }
    
    if(p->data == NULL){
        p->data = wpis;
        return wpis;
    }
    
    while(p->next != NULL){
        p = p->next;
    }
    
    Node* n = create_node();
    if(n == NULL){
        free(wpis);
        return NULL;
    }
    
    n->data = wpis;
    n->prev = p;
    p->next = n;
    
    printf("Dodano wpis ID: %d\n", wpis->id);
    return wpis;
}

bool can_delete_post(const Wpis* wpis) {
    if (wpis == NULL) {
        return false;
    }
    
    if (strcmp(wpis->status, STATUS_TO_VERIFY) == 0 ||
        strcmp(wpis->status, STATUS_IN_ANALYSIS) == 0) {
        return false;
    }
    
    return true;
}

int remove_post(Node* head, int id){
    Node* p = get_list_head(head);
    if (p == NULL) {
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    while(p != NULL){
        if(p->data != NULL && p->data->id == id){
            if(!can_delete_post(p->data)){
                printf("Błąd: nie można usunąć wpisu ID %d - status '%s' wymaga najpierw analizy\n", id, p->data->status);
                return 0;
            }
            
            if(p->prev != NULL){
                p->prev->next = p->next;
            }
            if(p->next != NULL){
                p->next->prev = p->prev;
            }
            
            free(p->data);
            free(p);
            return 1;
        }
        p = p->next;
    }
    
    printf("Błąd: nie znaleziono wpisu o ID: %d\n", id);
    return 0;
}

int remove_posts_by_status(Node* head, const char* status){
    if(status == NULL) {
        printf("Błąd: status jest NULL\n");
        return 0;
    }

    Node* p = get_list_head(head);
    if (p == NULL) {
        printf("Błąd: head jest NULL\n");
        return 0;
    }

    int removed_count = 0;

    while(p != NULL) {
        Node* next = p->next;
        
        if(p->data != NULL && strcmp(p->data->status, status) == 0) {

            if(p->prev != NULL) {
                p->prev->next = p->next;
            }
            if(p->next != NULL) {
                p->next->prev = p->prev;
            }

            free(p->data);
            free(p);
            removed_count++;
        }
        
        p = next;
    }

    if(removed_count == 0) {
        printf("Nie znaleziono wpisów ze statusem: %s\n", status);
    } else {
        printf("Usunięto %d wpis(ów) ze statusem: %s\n", removed_count, status);
    }
    
    return removed_count;
}

Wpis* find_post_by_id(Node* head, int id) {
    Node* p = get_list_head(head);
    if (p == NULL) {
        return NULL;
    }
    
    while (p != NULL) {
        if (p->data != NULL && p->data->id == id) {
            return p->data;
        }
        p = p->next;
    }
    
    return NULL;
}

int update_post_status(Node* head, int id, const char* new_status) {
    if (!validate_status(new_status)) {
        printf("Błąd: nieprawidłowy status\n");
        return 0;
    }
    
    Wpis* p = find_post_by_id(head, id);
    if (p == NULL) {
        printf("Błąd: nie znaleziono wpisu o ID: %d\n", id);
        return 0;
    }
    
    strcpy(p->status, new_status);
    return 1;
}

int edit_post(Node* head, int id, int field, const void* new_value) {
    if (new_value == NULL) {
        printf("Błąd: nieprawidłowe parametry\n");
        return 0;
    }
    
    Wpis* wpis = find_post_by_id(head, id);
    if (wpis == NULL) {
        printf("Błąd: nie znaleziono wpisu o ID: %d\n", id);
        return 0;
    }
    
    switch (field) {
        case FIELD_AUTHOR:
            if (!validate_author((const char*)new_value)) return 0;
            strcpy(wpis->author, (const char*)new_value);
            break;
        
        case FIELD_CONTENT:
            if (!validate_content((const char*)new_value)) return 0;
            strcpy(wpis->content, (const char*)new_value);
            break;
        
        case FIELD_CATEGORY:
            if (!validate_category((const char*)new_value)) return 0;
            strcpy(wpis->category, (const char*)new_value);
            break;
        
        case FIELD_STATUS:
            if (!validate_status((const char*)new_value)) return 0;
            strcpy(wpis->status, (const char*)new_value);
            break;
        
        case FIELD_REPORT_COUNT:
            if (*(const int*)new_value < 0) return 0;
            wpis->report_count = *(const int*)new_value;
            break;
        
        default:
            printf("Błąd: nieznane pole: %d\n", field);
            return 0;
    }
    
    return 1;
}
