CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = tario

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

debug: CFLAGS += -g -DDEBUG
debug: clean all

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)
