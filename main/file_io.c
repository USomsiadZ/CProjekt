#include "file_io.h"
#include "list.h"
#include "post.h"
#include "constants.h"
#include <stdio.h>
#include <string.h>

int save_to_file(Node* head, const char* filename){
    if(filename == NULL){
        printf("Błąd: nazwa pliku jest NULL\n");
        return 0;
    }
    
    Node* p = get_list_head(head);
    if (p == NULL) {
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    FILE* f = fopen(filename, "w");
    if(!f){
        printf("Błąd otwierania pliku\n");
        return 0;
    }
    
    int count = 0;
    
    while(p != NULL){
        if(p->data != NULL){
            fprintf(f, "%d\n", p->data->id);
            fprintf(f, "%s\n", p->data->author);
            fprintf(f, "%s\n", p->data->content);
            fprintf(f, "%s\n", p->data->category);
            fprintf(f, "%d\n", p->data->report_count);
            fprintf(f, "%s\n", p->data->status);
            count++;
        }
        p = p->next;
    }
    
    fclose(f);
    printf("Zapisano %d wpisów do pliku: %s\n", count, filename);
    return count;
}

int save_to_file_bin(Node* head, const char* filename){
    if(filename == NULL){
        printf("Błąd: nazwa pliku jest NULL\n");
        return 0;
    }
    
    Node* p = get_list_head(head);
    if (p == NULL) {
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    FILE* f = fopen(filename, "wb");
    if(!f){
        perror("Błąd otwierania pliku");
        return 0;
    }
    
    int count = 0;
    
    // Zapis binarny - zapisz każdy wpis używając fwrite
    while(p != NULL){
        if(p->data != NULL){
            fwrite(p->data, sizeof(Wpis), 1, f);
            count++;
        }
        p = p->next;
    }
    
    fclose(f);
    printf("Zapisano %d wpis(ów) do pliku binarnego: %s\n", count, filename);
    return count;
}

int load_from_file(Node* head, const char* filename){
    if(filename == NULL){
        printf("Błąd: nazwa pliku jest NULL\n");
        return 0;
    }
    
    if(head == NULL){
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    FILE* f = fopen(filename, "r");
    if(!f){
        printf("Błąd otwierania pliku: %s\n", filename);
        return 0;
    }
    
    int count = 0;
    int id;
    char author[MAX_AUTHOR_LEN];
    char content[MAX_CONTENT_LEN];
    char category[MAX_CATEGORY_LEN];
    int report_count;
    char status[MAX_STATUS_LEN];
    
    // Wczytaj wszystkie wpisy z pliku
    while(fscanf(f, "%d\n", &id) == 1){
        if(fgets(author, sizeof(author), f) == NULL) break;
        if(fgets(content, sizeof(content), f) == NULL) break;
        if(fgets(category, sizeof(category), f) == NULL) break;
        if(fscanf(f, "%d\n", &report_count) != 1) break;
        if(fgets(status, sizeof(status), f) == NULL) break;
        
        // Usuń znaki nowej linii
        size_t len = strlen(author);
        if(len > 0 && author[len-1] == '\n') author[len-1] = '\0';
        len = strlen(content);
        if(len > 0 && content[len-1] == '\n') content[len-1] = '\0';
        len = strlen(category);
        if(len > 0 && category[len-1] == '\n') category[len-1] = '\0';
        len = strlen(status);
        if(len > 0 && status[len-1] == '\n') status[len-1] = '\0';
        
        // Dodaj wpis do listy
        Wpis* added = add_post(head, author, content, category, status);
        if(added != NULL){
            // Ustaw ID i report_count ręcznie, bo add_post generuje nowe ID
            added->id = id;
            added->report_count = report_count;
            count++;
        }
    }
    
    fclose(f);
    printf("Wczytano %d wpis(ów) z pliku: %s\n", count, filename);
    return count;
}

int load_from_file_bin(Node* head, const char* filename){
    if(filename == NULL){
        printf("Błąd: nazwa pliku jest NULL\n");
        return 0;
    }
    
    if(head == NULL){
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    FILE* f = fopen(filename, "rb");
    if(!f){
        printf("Błąd otwierania pliku: %s\n", filename);
        return 0;
    }
    
    int count = 0;
    Wpis wpis;
    
    // Wczytaj wszystkie wpisy z pliku binarnego
    while(fread(&wpis, sizeof(Wpis), 1, f) == 1){
        // Dodaj wpis do listy
        Wpis* added = add_post(head, wpis.author, wpis.content, wpis.category, wpis.status);
        if(added != NULL){
            // Ustaw ID i report_count ręcznie, bo add_post generuje nowe ID
            added->id = wpis.id;
            added->report_count = wpis.report_count;
            count++;
        }
    }
    
    fclose(f);
    printf("Wczytano %d wpis(ów) z pliku binarnego: %s\n", count, filename);
    return count;
}
