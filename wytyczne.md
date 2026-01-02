# Wytyczne do projektu

## 1. Wymagania ogólne

Od 11. tygodnia semestru studenci realizują implementację większego zadania programistycznego. Treść projektu zostaje ogłoszona przez prowadzącego podczas zajęć.

Studenci zobowiązani są do systematycznego raportowania postępów prac w repozytorium kodu (np. GitHub, GitLab, Bitbucket).

Studenci pracują w repozytorium, gdzie widać historię commitów.

Raportowanie odbywa się poprzez regularne aktualizacje kodu i krótkie opisy commitów.

**Uwagi:**
- Brak wykazania regularności pracy skutkuje odjęciem **3 pkt**.
- Ostateczny termin oddania projektu przypada na **14. zajęcia**.
- Prezentacja oraz ocena projektów odbywają się podczas **14. i 15. zajęć**.
- Maksymalna liczba punktów, którą można uzyskać za projekt wynosi **30 pkt**.
- Brak wykorzystania list (jako struktur dynamicznych) skutkuje odjęciem **8 pkt**.
- Brak wykorzystania pamięci dynamicznej i przechowywanie danych na stosie skutkuje odjęciem **5 pkt**.
- Brak wykorzystania plików tekstowych lub binarnych skutkuje odjęciem **5 pkt**.
- Oddanie projektu po 14. zajęciach skutkuje odjęciem **5 punktów**.
- W celu uzyskania zaliczenia, za projekt należy zdobyć przynajmniej **15 punktów**.

**Ważne:** Projekt oceniany jest wyłącznie wtedy, gdy student potrafi wykazać się znajomością przedstawionego rozwiązania. Jeśli prowadzący stwierdzi brak samodzielności w przygotowaniu projektu lub brak znajomości jego treści i działania, praca nie zostaje oceniona pozytywnie i student otrzymuje **0 punktów**.

## 2. Wymagania funkcjonalne

### Reprezentacja danych

- Program musi wykorzystywać **typ strukturalny** do przechowywania informacji o pojedynczym obiekcie.
- Struktura powinna zawierać pola tekstowe (o określonych maksymalnych długościach) oraz pola liczbowe lub logiczne (zgodnie z tematyką projektu).
- Kolekcja obiektów musi być przechowywana w **pamięci typu sterta**.

### Zarządzanie kolekcją obiektów

- Program musi umożliwiać **dodawanie** nowych obiektów.
- Program musi umożliwiać **edycję** wybranego obiektu, z uwzględnieniem ewentualnych ograniczeń (np. pola nieedytowalne).
- Program musi umożliwiać **usuwanie** pojedynczego obiektu oraz usuwanie wielu obiektów spełniających wybrane kryterium.
- Program musi uniemożliwiać usuwanie obiektów, które spełniają określone warunki wykluczające (jeśli przewiduje to temat projektu).

### Wyszukiwanie i filtrowanie

- Program musi umożliwiać wyszukiwanie obiektów według co najmniej **dwóch różnych kryteriów** (jednego pola tekstowego i jednego pola liczbowego).
- Wyszukiwanie tekstowe musi umożliwiać dopasowanie **pełne lub prefiksowe**.
- Wyniki wyszukiwania muszą obejmować **wszystkie obiekty** spełniające kryteria.

### Sortowanie

- Program musi umożliwiać sortowanie **alfabetyczne** według wybranego pola tekstowego.
- Program musi umożliwiać sortowanie według wybranego pola liczbowego.
- Sortowanie może być zrealizowane jako przepisanie danych do nowej struktury z zachowaniem wymaganego porządku - nie ma konieczności przestawiania elementów na strukturze oryginalnej.
- Sortowanie musi działać poprawnie niezależnie od liczby obiektów.

### Prezentacja danych

- Program musi umożliwiać wyświetlanie pełnej listy obiektów oraz wyników wyszukiwania w formie **czytelnych zestawień**.

### Trwałość danych

- Program musi umożliwiać **zapis** całej kolekcji obiektów do pliku tekstowego lub binarnego.
- Program musi umożliwiać **odczyt** danych z pliku i odtworzenie kolekcji obiektów w pamięci.
- Program musi obsługiwać błędy związane z plikami (brak pliku, niezgodny format, przerwane dane) w sposób **kontrolowany**.
- Ścieżki/nazwy plików używane do zapisu/odczytu muszą być przekazywane jako **parametry wywołania programu**.

## 3. Wymagania niefunkcjonalne

### Jakość kodu

- Kod programu musi być **modularny**, a funkcje logicznie rozdzielone zgodnie z odpowiedzialnościami.
- Program musi być podzielony na **pliki źródłowe i nagłówkowe**, zgodnie z zasadami modularności w C.
- Program musi wykorzystywać **bezpieczne funkcje** wejścia-wyjścia, walidować dane wejściowe oraz unikać przepełnień buforów.
- Program musi unikać **wycieków pamięci** i niepoprawnych operacji na wskaźnikach.

### Interfejs użytkownika

- Interfejs programu musi być oparty na czytelnym, intuicyjnym **menu tekstowym** obejmującym wszystkie funkcje systemu.
- Komunikaty programu muszą być **jednoznaczne, zrozumiałe** i informować użytkownika o błędach oraz sposobach ich rozwiązania.

### Niezawodność

- Program **nie może kończyć działania** na skutek błędnego wejścia; wszystkie takie sytuacje muszą być obsługiwane komunikatami i ponawianiem operacji.
- Program musi działać stabilnie dla kolekcji liczącej co najmniej **kilkaset obiektów**.
