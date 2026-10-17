CC = clang
CFLAGS = -Wall -Wextra -std=c11
CFLAGS = -Iinclude

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
TARGET = quiz-core

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(TARGET)
