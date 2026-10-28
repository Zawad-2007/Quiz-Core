CC = clang
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
TARGET = quiz-core

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(TARGET)
