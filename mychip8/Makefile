CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = chip8

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

TARGET = chip8

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)


# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Check for memory leaks with valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the shell (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and rebuild"
	@echo "  run      - Build and run the shell"
	@echo "  valgrind - Run with valgrind memory checker"
	@echo "  help     - Show this help message"

.PHONY: all clean rebuild run valgrind help