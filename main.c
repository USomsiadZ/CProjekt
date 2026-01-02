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

#define STATUS_TO_VERIFY "do weryfikacji"
#define STATUS_IN_ANALYSIS "w trakcie analizy"
#define STATUS_APPROVED "zatwierdzone"
#define STATUS_DELETED "usunięte"

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

// walidacja
bool validate_author(const char* author);
bool validate_content(const char* content);
bool validate_category(const char* category);
bool validate_status(const char* status);
int get_next_id(Node* head);

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
    if(head == NULL){
        printf("Błąd: head jest NULL\n");
        return NULL;
    }
    
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
    

    Node* p = head;
    while(p->prev != NULL){
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
int remove_post(Node* head, int id){
    if(head == NULL){
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    Node* p = head;
    while(p->prev != NULL){
        p = p->prev;
    }
    while(p != NULL){
        if(p->data->id == id){
            Node* next = p->next;
            free(p->data);
            free(p);
            return 1;
        }
        p = p->next;
    }
    return 0;
}
int remove_posts_by_status(Node* head, const char* status){
    if(head == NULL) {
        printf("Błąd: head jest NULL\n");
        return 0;
    }
    
    if(status == NULL) {
        printf("Błąd: status jest NULL\n");
        return 0;
    }

    int removed_count = 0;
    Node* p = head;
    

    while(p->prev != NULL) {
        p = p->prev;
    }

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
Wpis* find_post_by_id(Node* head, int id);

// edycja postów
int edit_post(Node* head, int id, const char* name, const void* new_value);
int update_post_status(Node* head, int id, const char* new_status);
bool can_delete_post(const Wpis* wpis);

// wyszukiwanie
Node* search_by_author(Node* head, const char* author, bool prefix_match);
Node* search_by_report_count(Node* head, int count);
Node* search_by_category(Node* head, const char* category);
Node* search_by_status(Node* head, const char* status);

// sortowanie
Node* sort_by_author(Node* head);
Node* sort_by_report_count(Node* head);
Node* sort_by_id(Node* head);
Node* sort_by_category(Node* head);

// wyświetlanie
void display_post(const Wpis* wpis);
void display_all_posts(Node* head);
void display_search_results(Node* results);
void display_post_summary(Node* head);

// pliki
int save_to_file(Node* head, const char* filename, bool binary);
int load_from_file(Node* head, const char* filename, bool binary);



// menu i interfejs
void show_add_post_menu(Node* head);
void show_edit_post_menu(Node* head);
void show_delete_post_menu(Node* head);
void show_search_menu(Node* head);
void show_sort_menu(Node* head);
void show_file_menu(Node* head, const char* filename, bool binary);

























void show_menu(void){
    printf("\n=== SYSTEM MODERACJI POSTÓW QUICKTALK ===\n");
    printf("1. Dodaj nowy p\n");
    printf("2. Edytuj p\n");
    printf("3. Usuń p\n");
    printf("4. Wyświetl wszystkie posty\n");
    printf("5. Wyszukaj posty\n");
    printf("6. Sortuj posty\n");
    printf("7. Zapisz/Odczytaj z pliku\n");
    printf("0. Wyjście\n");
    printf("Wybierz opcję: ");
}

int option_menu(Node* head){
    int wybor;
    int result;
    printf("\n");
    printf("Podaj numer opcji: ");
    result = scanf("%d", &wybor);

    while(getchar() != '\n');

    if(result != 1 || wybor < 0 || wybor > 7) {
        printf("Nieznana opcja. Wybierz poprawną liczbę z menu.\n");
        return 1;
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
            show_sort_menu(head);
            break;
        case 7:
            show_file_menu(head, "posts.txt", false);
            break;
        case 0:
            printf("Wyjście z programu\n");
            return 0;
        default:
            printf("Nieznana opcja. Wybierz poprawną liczbę z menu.\n");
            break;
    }
    return 1;
}


int main(int argc, char* argv[]){
    Node* head = create_node();
    while(1){
        show_menu();
        if(option_menu(head) == 0){
            break;
        }
        // kod
    }
    destroy_node(head);
    return 0;
}