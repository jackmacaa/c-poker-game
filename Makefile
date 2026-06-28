CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c11
CPPFLAGS := -Iinclude

TARGET := bin/poker.exe
OBJDIR := bin/obj

SRC := src/poker.c src/card.c src/deck.c src/hand_eval.c src/holdem.c
OBJ := $(SRC:src/%.c=$(OBJDIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJ) -o $(TARGET)


$(OBJDIR)/%.o: src/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
