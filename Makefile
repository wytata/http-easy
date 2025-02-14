CC = gcc
INCLUDE_DIR = src/
CFLAGS = -I $(INCLUDE_DIR)
CFLAGS += -Wall -Wextra -g

TARGET = http-easy
SRC_DIR = src
BUILD_DIR = build
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
OBJ_NO_MAIN = $(filter-out $(BUILD_DIR)/main.o, $(OBJ))

TEST_DIR = test
TEST_BUILD_DIR = $(TEST_DIR)/build
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TESTS = $(patsubst $(TEST_DIR)/%.c, $(TEST_BUILD_DIR)/%, $(TEST_SRCS))

all: $(BUILD_DIR) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

$(TEST_BUILD_DIR)/%: $(TEST_DIR)/%.c $(TEST_BUILD_DIR) $(OBJ_NO_MAIN)
	$(CC) $(CFLAGS) $< $(OBJ_NO_MAIN) -o $@ 

test: $(TESTS) $(TEST_BUILD_DIR)
	for test in $(TESTS); do ./$$test; done

clean:
	rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR) $(TARGET)

.PHONY: all clean test

