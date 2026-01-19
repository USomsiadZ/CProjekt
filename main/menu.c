#include "menu.h"
#include "constants.h"
#include "post.h"
#include "search.h"
#include "sort.h"
#include "display.h"
#include "file_io.h"
#include <stdio.h>
#include <string.h>

void read_string(char* buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    int len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') {
        buf[len-1] = '\0';
    } else if (len == size - 1) {
        while(getchar() != '\n');
    }
}

int read_int() {
    int value;
    if (scanf("%d", &value) != 1) {
        while(getchar() != '\n');
        return -1;
    }
    while(getchar() != '\n');
    return value;
}

void get_category(char* category) {
    printf("Wybierz kategorię:\n");
    printf("1. %s\n", CATEGORY_SPAM);
    printf("2. %s\n", CATEGORY_HATE);
    printf("3. %s\n", CATEGORY_PROFANITY);
    printf("4. %s\n", CATEGORY_FAKE_NEWS);
    printf("5. %s\n", CATEGORY_INAPPROPRIATE);
    printf("6. %s\n", CATEGORY_OK);
    printf("Wybierz: ");
    int w = read_int();
    switch(w) {
        case 1: strcpy(category, CATEGORY_SPAM); break;
        case 2: strcpy(category, CATEGORY_HATE); break;
        case 3: strcpy(category, CATEGORY_PROFANITY); break;
        case 4: strcpy(category, CATEGORY_FAKE_NEWS); break;
        case 5: strcpy(category, CATEGORY_INAPPROPRIATE); break;
        case 6: strcpy(category, CATEGORY_OK); break;
        default: category[0] = '\0';
    }
}

void get_status(char* status) {
    printf("Wybierz status:\n");
    printf("1. %s\n", STATUS_TO_VERIFY);
    printf("2. %s\n", STATUS_IN_ANALYSIS);
    printf("3. %s\n", STATUS_APPROVED);
    printf("4. %s\n", STATUS_DELETED);
    printf("Wybierz: ");
    int w = read_int();
    switch(w) {
        case 1: strcpy(status, STATUS_TO_VERIFY); break;
        case 2: strcpy(status, STATUS_IN_ANALYSIS); break;
        case 3: strcpy(status, STATUS_APPROVED); break;
        case 4: strcpy(status, STATUS_DELETED); break;
        default: status[0] = '\0';
    }
}

void show_add_post_menu(Node* head){
    char author[MAX_AUTHOR_LEN];
    char content[MAX_CONTENT_LEN];
    char category[MAX_CATEGORY_LEN];
    char status[MAX_STATUS_LEN];
    
    printf("\n=== DODAWANIE NOWEGO POSTA ===\n");
    printf("Podaj autora: ");
    read_string(author, sizeof(author));
    printf("Podaj treść: ");
    read_string(content, sizeof(content));
    get_category(category);
    if (!category[0]) {
        printf("Nieprawidłowy wybór kategorii\n");
        return;
    }
    get_status(status);
    if (!status[0]) {
        printf("Nieprawidłowy wybór statusu\n");
        return;
    }
    add_post(head, author, content, category, status);
}

void show_edit_post_menu(Node* head){
    printf("\n=== EDYCJA POSTA ===\n");
    printf("Podaj ID posta do edycji: ");
    int id = read_int();
    if (id < 0) return;
    
    printf("Wybierz pole do edycji:\n");
    printf("0. Autor\n");
    printf("1. Treść\n");
    printf("2. Kategoria\n");
    printf("3. Status\n");
    printf("4. Liczba zgłoszeń\n");
    printf("Wybierz: ");
    int f = read_int();
    if (f < 0 || f > 4) {
        printf("Nieprawidłowy wybór pola\n");
        return;
    }
    
    if (f == FIELD_AUTHOR) {
        char author[MAX_AUTHOR_LEN];
        printf("Podaj nowego autora: ");
        read_string(author, sizeof(author));
        if (edit_post(head, id, f, author)) printf("Zaktualizowano autora\n");
    } else if (f == FIELD_CONTENT) {
        char content[MAX_CONTENT_LEN];
        printf("Podaj nową treść: ");
        read_string(content, sizeof(content));
        if (edit_post(head, id, f, content)) printf("Zaktualizowano treść\n");
    } else if (f == FIELD_CATEGORY) {
        char category[MAX_CATEGORY_LEN];
        get_category(category);
        if (!edit_post(head, id, f, category)) {
            printf("Nieprawidłowy wybór kategorii\n");
        } else {
            printf("Zaktualizowano kategorię\n");
        }
    } else if (f == FIELD_STATUS) {
        char status[MAX_STATUS_LEN];
        get_status(status);
        if (!edit_post(head, id, f, status)) {
            printf("Nieprawidłowy wybór statusu\n");
        } else {
            printf("Zaktualizowano status\n");
        }
    } else if (f == FIELD_REPORT_COUNT) {
        printf("Podaj nową liczbę zgłoszeń: ");
        int count = read_int();
        if (count >= 0 && edit_post(head, id, f, &count)) {
            printf("Zaktualizowano liczbę zgłoszeń\n");
        }
    }
}

void show_delete_post_menu(Node* head){
    printf("\n=== USUWANIE POSTA ===\n");
    printf("1. Usuń pojedynczy post (po ID)\n");
    printf("2. Usuń posty po statusie\n");
    printf("Wybierz: ");
    int w = read_int();
    
    if (w == 1) {
        printf("Podaj ID posta do usunięcia: ");
        int id = read_int();
        if (id >= 0) remove_post(head, id);
    } else if (w == 2) {
        char status[MAX_STATUS_LEN];
        get_status(status);
        //pusty
        if (status[0]) {
            remove_posts_by_status(head, status);
        } else {
            printf("Nieprawidłowy wybór statusu\n");
        }
    } else {
        printf("Nieprawidłowy wybór\n");
    }
}

void show_search_menu(Node* head){
    printf("\n=== WYSZUKIWANIE POSTÓW ===\n");
    printf("1. Wyszukaj po autorze\n");
    printf("2. Wyszukaj po liczbie zgłoszeń\n");
    printf("Wybierz: ");
    int w = read_int();
    
    if (w == 1) {
        printf("1. Dopasowanie pełne\n");
        printf("2. Dopasowanie prefiksowe\n");
        printf("Wybierz: ");
        int match_type = read_int();
        if (match_type < 1 || match_type > 2) return;
        
        char author[MAX_AUTHOR_LEN];
        printf("Podaj autora: ");
        read_string(author, sizeof(author));
        search_by_author(head, author, match_type == 2);
    } else if (w == 2) {
        printf("Podaj liczbę zgłoszeń: ");
        int count = read_int();
        if (count >= 0) search_by_report_count(head, count);
    } else {
        printf("Nieprawidłowy wybór\n");
    }
}

Node* show_sort_menu(Node* head){
    printf("\n=== SORTOWANIE POSTÓW ===\n");
    printf("1. Sortuj po autorze\n");
    printf("2. Sortuj po liczbie zgłoszeń\n");
    printf("Wybierz: ");
    int w = read_int();
    
    if (w == 1) {
        Node* sorted = sort_by_author(head);
        if (sorted != NULL) {
            printf("Posortowano posty po autorze\n");
            display_all_posts(sorted);
            return sorted;
        }
    } else if (w == 2) {
        Node* sorted = sort_by_report_count(head);
        if (sorted != NULL) {
            printf("Posortowano posty po liczbie zgłoszeń\n");
            display_all_posts(sorted);
            return sorted;
        }
    } else {
        printf("Nieprawidłowy wybór\n");
    }
    return head;
}

void show_file_menu(Node* head, const char* filename){
    printf("\n=== ZARZĄDZANIE PLIKAMI (TEKSTOWE) ===\n");
    printf("1. Zapisz do pliku\n2. Wczytaj z pliku\n");
    printf("Wybierz: ");
    int w = read_int();
    
    if (w == 1) {
        save_to_file(head, filename);
    } else if (w == 2) {
        load_from_file(head, filename);
    } else {
        printf("Nieprawidłowy wybór\n");
    }
}

void show_file_menu_bin(Node* head, const char* filename){
    printf("\n=== ZARZĄDZANIE PLIKAMI (BINARNE) ===\n");
    printf("1. Zapisz do pliku\n");
    printf("2. Wczytaj z pliku\n");
    printf("Wybierz: ");
    int w = read_int();
    
    if (w == 1) {
        save_to_file_bin(head, filename);
    } else if (w == 2) {
        load_from_file_bin(head, filename);
    } else {
        printf("Nieprawidłowy wybór\n");
    }
}

void show_menu(void){
    printf("\n=== SYSTEM MODERACJI POSTÓW QUICKTALK ===\n");
    printf("1. Dodaj nowy post\n");
    printf("2. Edytuj post\n");
    printf("3. Usuń post\n");
    printf("4. Wyświetl wszystkie posty\n");
    printf("5. Wyszukaj posty\n");
    printf("6. Sortuj posty\n");
    printf("7. Zapisz/Odczytaj z pliku\n");
    printf("0. Wyjście\n");
    printf("Wybierz opcję: ");
}

Node* option_menu(Node* head, const char* filename){
    int wybor = read_int();

    if(wybor < 0 || wybor > 7) {
        printf("Nieznana opcja. Wybierz poprawną liczbę z menu.\n");
        return head;
    }

    switch(wybor) {
        case 1:
            show_add_post_menu(head);
            break;
        case 2:
            show_edit_post_menu(head);
            break;
        case 3:
            show_delete_post_menu(head);
            break;
        case 4:
            display_all_posts(head);
            break;
        case 5:
            show_search_menu(head);
            break;
        case 6:
            head = show_sort_menu(head);
            break;
        case 7:
            printf("\n=== ZARZĄDZANIE PLIKAMI ===\n");
            printf("1. Plik tekstowy\n");
            printf("2. Plik binarny\n");
            printf("Wybierz typ pliku: ");
            int file_type = read_int();
            
            if (file_type == 1) {
                show_file_menu(head, filename);
            } else if (file_type == 2) {
                show_file_menu_bin(head, filename);
            } else {
                printf("Nieprawidłowy wybór\n");
            }
            break;
        case 0:
            printf("Wyjście z programu\n");
            return NULL;
        default:
            printf("Nieznana opcja. Wybierz poprawną liczbę z menu.\n");
            break;
    }
    return head;
}
