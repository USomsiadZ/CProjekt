/*
10. Moderacyjna Baza Postów Social-Media
Platforma „QuickTalk” to jedno z najpopularniejszych mediów społecznościowych na świecie. Każdego dnia użytkownicy publikują setki tysięcy krótkich wpisów komentujących rzeczywistość, wydarzenia, trendy i memy. Aby utrzymać platformę w bezpiecznym i przyjaznym stanie, działa szybki zespół moderatorów. Do ich pracy potrzebny jest sprawny system katalogowania i oceny postów, które zostały zgłoszone przez innych użytkowników.

Twoim zadaniem jest stworzenie programu umożliwiającego zarządzanie danymi o zgłoszonych postach: ich rejestracją, wyszukiwaniem, modyfikacją, sortowaniem, usuwaniem oraz zapisem/odczytem z pliku. System ułatwia szybkie sortowanie i ocenę treści tak, aby mem o kotach nie trafił na nodeę „poważne zagrożenie dla demokracji”.

Każdy p posiada minimalny (może być rozszerzony) zestaw cech, które muszą zostać zapisane w systemie:

numeru ID wpisu - wewnętrzny unikalny identyfikator wszystkich wpisów w systemie (nadawany automatycznie w momencie dodania wpisu);
nazwa autora - nazwa użytkownika publikującego p, o maksymalnej długości 100 znaków. Pole może zawierać pseudonim, nazwę konta lub inny identyfikator;
treść posta - tekst wpisu, o maksymalnej długości zależnej od projektu (np. do 280 znaków). Treść powinna być przechowywana jako tekst i nie powinna być traktowana jako pole kluczowe;
kategoria zgłoszenia - powód, dla którego p został zgłoszony: spam, hejt, wulgaryzmy, fejk-news, nieodpowiednie treści. 
liczba zgłoszeń - liczba określająca, ile razy użytkownicy zgłosili dany p do moderacji;
status moderacji - aktualny stan wpisu, np.: do weryfikacji, w trakcie analizy, zatwierdzone, usunięte. Posty, które nie zostały jeszcze przeanalizowane przez moderatora, nie mogą zostać usunięte. Najpierw należy zmienić ich status na zatwierdzone lub usunięte w wyniku moderacji. Program powinien zablokować próbę usunięcia i wyświetlić stosowny komunikat.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_AUTHOR_LEN 100
#define MAX_CONTENT_LEN 280
#define MAX_CATEGORY_LEN 64
#define MAX_STATUS_LEN 64

#define CATEGORY_SPAM "spam"
#define CATEGORY_HATE "hejt"
#define CATEGORY_PROFANITY "wulgaryzmy"
#define CATEGORY_FAKE_NEWS "fejk-news"
#define CATEGORY_INAPPROPRIATE "nieodpowiednie treści"
#define CATEGORY_OK "ok"

#define STATUS_TO_VERIFY "do weryfikacji"
#define STATUS_IN_ANALYSIS "w trakcie analizy"
#define STATUS_APPROVED "zatwierdzone"
#define STATUS_DELETED "usunięte"

#define FIELD_AUTHOR 0
#define FIELD_CONTENT 1
#define FIELD_CATEGORY 2
#define FIELD_STATUS 3
#define FIELD_REPORT_COUNT 4

typedef struct Wpis {
    int id;
    char author[MAX_AUTHOR_LEN];
    char content[MAX_CONTENT_LEN];
    char category[MAX_CATEGORY_LEN];
    int report_count;
    char status[MAX_STATUS_LEN];
} Wpis;

typedef struct Node {
    Wpis* data;
    struct Node* prev;
    struct Node* next;
} Node;

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
// walidacja
bool validate_author(const char* author) {
    if (author == NULL) {
        return false;
    }
    
    size_t len = strlen(author);
    
    if (len == 0) {
        return false;
    }
    
    if (len >= MAX_AUTHOR_LEN) {
        return false;
    }
    
    return true;
}
bool validate_content(const char* content) {
    if (content == NULL) {
        return false;
    }
    
    size_t len = strlen(content);
    
    if (len == 0) {
        return false;
    }
    // '\0'
    if (len >= MAX_CONTENT_LEN) {
        return false;
    }
    
    return true;
}
bool validate_category(const char* category) {
    if (category == NULL) {
        return false;
    }
    
    if (strcmp(category, CATEGORY_SPAM) == 0 ||
        strcmp(category, CATEGORY_HATE) == 0 ||
        strcmp(category, CATEGORY_PROFANITY) == 0 ||
        strcmp(category, CATEGORY_FAKE_NEWS) == 0 ||
        strcmp(category, CATEGORY_INAPPROPRIATE) == 0 ||
        strcmp(category, CATEGORY_OK) == 0) {
        return true;
    }
    
    return false;
}
bool validate_status(const char* status) {
    if (status == NULL) {
        return false;
    }
    
    if (strcmp(status, STATUS_TO_VERIFY) == 0 ||
        strcmp(status, STATUS_IN_ANALYSIS) == 0 ||
        strcmp(status, STATUS_APPROVED) == 0 ||
        strcmp(status, STATUS_DELETED) == 0) {
        return true;
    }
    
    return false;
}
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



// funkcje do zarządzania node
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

// operacje na postach
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



// wyszukiwanie
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

// sortowanie
// https://visualgo.net/en/sorting
/*
[5, 2, 8, 1, 9]
 ^  ^  porównaj 5 > 2? TAK → zamień
[2, 5, 8, 1, 9]
    ^  ^  porównaj 5 > 8? NIE
[2, 5, 8, 1, 9]
       ^  ^  porównaj 8 > 1? TAK → zamień
[2, 5, 1, 8, 9]
          ^  ^  porównaj 8 > 9? NIE
[2, 5, 1, 8, 9]  ← koniec przejścia, 9 na końcu
*/
void swap_wpis(Wpis** a, Wpis** b) {
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

// wyświetlanie
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

// pliki
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






// menu i interfejs
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


int main(int argc, char* argv[]){
    const char* filename;
    
    if(argc < 2){
        printf("Używam domyślnej nazwy pliku: posts.txt\n");
        filename = "posts.txt";
    } else {
        filename = argv[1];
        printf("Używam pliku: %s\n", filename);
    }
    
    Node* head = create_node();
    if(head == NULL){
        printf("Błąd: nie udało się utworzyć listy\n");
        return 1;
    }
    
    while(1){
        show_menu();
        head = option_menu(head, filename);
        if(head == NULL){
            break;
        }
    }
    
    if(head != NULL){
        destroy_node(head);
    }
    return 0;
}