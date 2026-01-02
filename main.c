/*
10. Moderacyjna Baza Postów Social-Media
Platforma „QuickTalk” to jedno z najpopularniejszych mediów społecznościowych na świecie. Każdego dnia użytkownicy publikują setki tysięcy krótkich wpisów komentujących rzeczywistość, wydarzenia, trendy i memy. Aby utrzymać platformę w bezpiecznym i przyjaznym stanie, działa szybki zespół moderatorów. Do ich pracy potrzebny jest sprawny system katalogowania i oceny postów, które zostały zgłoszone przez innych użytkowników.

Twoim zadaniem jest stworzenie programu umożliwiającego zarządzanie danymi o zgłoszonych postach: ich rejestracją, wyszukiwaniem, modyfikacją, sortowaniem, usuwaniem oraz zapisem/odczytem z pliku. System ułatwia szybkie sortowanie i ocenę treści tak, aby mem o kotach nie trafił na nodeę „poważne zagrożenie dla demokracji”.

Każdy wpis posiada minimalny (może być rozszerzony) zestaw cech, które muszą zostać zapisane w systemie:

numeru ID wpisu - wewnętrzny unikalny identyfikator wszystkich wpisów w systemie (nadawany automatycznie w momencie dodania wpisu);
nazwa autora - nazwa użytkownika publikującego wpis, o maksymalnej długości 100 znaków. Pole może zawierać pseudonim, nazwę konta lub inny identyfikator;
treść posta - tekst wpisu, o maksymalnej długości zależnej od projektu (np. do 280 znaków). Treść powinna być przechowywana jako tekst i nie powinna być traktowana jako pole kluczowe;
kategoria zgłoszenia - powód, dla którego wpis został zgłoszony: spam, hejt, wulgaryzmy, fejk-news, nieodpowiednie treści. 
liczba zgłoszeń - liczba określająca, ile razy użytkownicy zgłosili dany wpis do moderacji;
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
    Wpis* wpis;
    struct Node* prev;
    struct Node* next;
} Node;


// funkcje do zarządzania node
Node* create_node(void){
    Node* node = calloc(1, sizeof(Node));
    if(node == NULL){
        printf("Błąd alokacji pamięci\n");
        return NULL;
    }
    node->wpis = NULL;
    node->prev = NULL;
    node->next = NULL;
    return node;
}
void destroy_node(Node* node) {
    if (node == NULL) {
        printf("Błąd: node jest NULL\n");
        return;
    }
    // znajdź początek listy
    Node* head = node;
    while (head->prev != NULL) {
        head = head->prev;
    }
    // usuń wszystkie węzły
    Node* p = head;
    while (p != NULL) {
        Node* next = p->next;
        if (p->wpis != NULL) {
            free(p->wpis);
        }
        free(p);
        p = next;
    }
}

// operacje na postach
Wpis* add_post(Node* node, const char* author, const char* content, 
               const char* category, const char* status);
int remove_post(Node* node, int id);
int remove_posts_by_criterion(Node* node, int criterion_type, const void* value);
Wpis* find_post_by_id(Node* node, int id);

// edycja postów
int edit_post(Node* node, int id, const char* name, const void* new_value);
int update_post_status(Node* node, int id, const char* new_status);
bool can_delete_post(const Wpis* wpis);

// wyszukiwanie
Node* search_by_author(Node* node, const char* author, bool prefix_match);
Node* search_by_report_count(Node* node, int count);
Node* search_by_category(Node* node, const char* category);
Node* search_by_status(Node* node, const char* status);

// sortowanie
Node* sort_by_author(Node* node);
Node* sort_by_report_count(Node* node);
Node* sort_by_id(Node* node);
Node* sort_by_category(Node* node);

// wyświetlanie
void display_post(const Wpis* wpis);
void display_all_posts(Node* node);
void display_search_results(Node* results);
void display_post_summary(Node* node);

// pliki
int save_to_file(Node* node, const char* filename, bool binary);
int load_from_file(Node* node, const char* filename, bool binary);


// walidacja i pomocnicze
bool validate_author(const char* author);
bool validate_content(const char* content);
bool validate_category(const char* category);
bool validate_status(const char* status);
int get_next_id(Node* node);

// menu i interfejs
void show_add_post_menu(Node* node);
void show_edit_post_menu(Node* node);
void show_delete_post_menu(Node* node);
void show_search_menu(Node* node);
void show_sort_menu(Node* node);
void show_file_menu(Node* node, const char* filename, bool binary);

























void show_menu(void){
    printf("\n=== SYSTEM MODERACJI POSTÓW QUICKTALK ===\n");
    printf("1. Dodaj nowy wpis\n");
    printf("2. Edytuj wpis\n");
    printf("3. Usuń wpis\n");
    printf("4. Wyświetl wszystkie posty\n");
    printf("5. Wyszukaj posty\n");
    printf("6. Sortuj posty\n");
    printf("7. Zapisz/Odczytaj z pliku\n");
    printf("0. Wyjście\n");
    printf("Wybierz opcję: ");
}

int option_menu(Node* node){
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
            show_add_post_menu(node);
            break;
        case 2:
            show_edit_post_menu(node);
            break;
        case 3:
            show_delete_post_menu(node);
            break;
        case 4:
            display_all_posts(node);
            break;
        case 5:
            show_search_menu(node);
            break;
        case 6:
            show_sort_menu(node);
            break;
        case 7:
            show_file_menu(node, "posts.txt", false);
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


int main(int nazwa, char* argv[]){
    Node* node = create_node();
    while(1){
        show_menu();
        if(option_menu(node) == 0){
            break;
        }
        // kod
    }
    destroy_node(node);
    return 0;
}