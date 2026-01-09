# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS :=

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := build

# Target
TARGET  := bananatcp

# Source files
SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/tun.c \
	$(SRC_DIR)/packet.c \
	$(SRC_DIR)/ipv4.c \
	$(SRC_DIR)/icmp.c \
	$(SRC_DIR)/udp.c \
	$(SRC_DIR)/tcp.c \
	$(SRC_DIR)/checksum.c

# Object files
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Include path
INCLUDES := -I$(INC_DIR)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create build directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild from scratch
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild
