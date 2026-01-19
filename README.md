# QuickTalk - System Moderacji Postów

System zarządzania zgłoszonymi postami w mediach społecznościowych.

## Struktura projektu

```
main/
├── constants.h       - Definicje stałych
├── types.h           - Struktury danych (Wpis, Node)
├── validation.h/c    - Walidacja danych wejściowych
├── list.h/c          - Operacje na liście dwukierunkowej
├── post.h/c          - Zarządzanie postami (CRUD)
├── search.h/c        - Wyszukiwanie postów
├── sort.h/c          - Sortowanie postów
├── display.h/c       - Wyświetlanie postów
├── file_io.h/c       - Operacje na plikach
├── menu.h/c          - Interfejs użytkownika
└── main.c            - Główna funkcja programu
```

## Kompilacja

Program używa Makefile do kompilacji:

```bash
make          # kompilacja projektu
make clean    # usunięcie plików obiektowych i wykonywalnych
make rebuild  # czyszczenie i ponowna kompilacja
```

## Uruchomienie

```bash
./quicktalk [nazwa_pliku]
```

Jeśli nazwa pliku nie jest podana, program użyje domyślnej nazwy `posts.txt`.

## Funkcjonalności

- Dodawanie nowych postów
- Edycja istniejących postów
- Usuwanie postów (z weryfikacją statusu)
- Wyświetlanie wszystkich postów
- Wyszukiwanie po autorze i liczbie zgłoszeń
- Sortowanie po autorze i liczbie zgłoszeń
- Zapis/odczyt z plików tekstowych i binarnych

## Wymagania

- Kompilator GCC
- Standard C11
