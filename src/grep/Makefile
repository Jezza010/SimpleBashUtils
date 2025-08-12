CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = s21_grep
SRC = s21_grep.c

all: $(TARGET)

$(TARGET): $(SRC) s21_grep.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

