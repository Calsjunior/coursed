CC = gcc
ifeq ($(shell which $(CC)),)
    CC := clang
endif
CFLAGS = -g -O0 -std=c99 -Iinclude
LDFLAGS = -lsqlite3

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = coursed


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
