CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include
TEST_DIR = tests
PREFIX = /usr/local

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = tario
TEST_TARGET = $(OBJ_DIR)/test_tario

.PHONY: all clean debug run test install uninstall check valgrind format help

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Debug build with symbols and DEBUG flag
debug: CFLAGS += -g -DDEBUG
debug: clean all

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Build and run the game
run: $(TARGET)
	./$(TARGET)

# Run tests
test: $(TEST_TARGET)
	@echo "Running test suite..."
	@$(TEST_TARGET)

# Build test executable
$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS)) | $(OBJ_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile test object files
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/test_%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Check code quality (compile with strict warnings)
check:
	@echo "Checking code quality..."
	$(CC) $(CFLAGS) -Werror -fsyntax-only $(wildcard $(SRC_DIR)/*.c)
	@echo "Code quality check passed!"

# Run memory leak detection
valgrind: debug
	@echo "Running valgrind memory leak check..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Format code (requires clang-format)
format:
	@if command -v clang-format > /dev/null; then \
		echo "Formatting source files..."; \
		clang-format -i $(wildcard $(SRC_DIR)/*.c) $(wildcard $(INCLUDE_DIR)/*.h); \
		echo "Formatting complete!"; \
	else \
		echo "clang-format not found. Install with: apt-get install clang-format"; \
	fi

# Install the game
install: $(TARGET)
	@echo "Installing $(TARGET) to $(PREFIX)/bin..."
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)
	@echo "Installation complete! Run with: $(TARGET)"

# Uninstall the game
uninstall:
	@echo "Uninstalling $(TARGET) from $(PREFIX)/bin..."
	rm -f $(PREFIX)/bin/$(TARGET)
	@echo "Uninstallation complete!"

# Display help information
help:
	@echo "Tario Makefile - Available targets:"
	@echo ""
	@echo "  make              Build the game (release mode)"
	@echo "  make debug        Build with debug symbols and DEBUG flag"
	@echo "  make run          Build and run the game"
	@echo "  make clean        Remove all build artifacts"
	@echo "  make test         Run test suite"
	@echo "  make check        Check code quality (strict compilation)"
	@echo "  make valgrind     Run memory leak detection"
	@echo "  make format       Format code with clang-format"
	@echo "  make install      Install to $(PREFIX)/bin (may require sudo)"
	@echo "  make uninstall    Uninstall from $(PREFIX)/bin (may require sudo)"
	@echo "  make help         Display this help message"
	@echo ""
	@echo "Compiler: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Install prefix: $(PREFIX)"
