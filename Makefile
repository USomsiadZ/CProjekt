CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Imain
TARGET = quicktalk
SRC_DIR = main

# Pliki źródłowe
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/validation.c \
       $(SRC_DIR)/list.c \
       $(SRC_DIR)/post.c \
       $(SRC_DIR)/search.c \
       $(SRC_DIR)/sort.c \
       $(SRC_DIR)/display.c \
       $(SRC_DIR)/file_io.c \
       $(SRC_DIR)/menu.c

# Pliki obiektowe
OBJS = $(SRCS:.c=.o)

# Reguła główna
all: $(TARGET)

# Linkowanie
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Kompilacja plików .c do .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Czyszczenie
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild
rebuild: clean all

.PHONY: all clean rebuild
