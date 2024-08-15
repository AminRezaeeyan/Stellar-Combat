# Makefile for Stellar Combat game

# Project settings
PROJECT_NAME = StellarCombat
CC = gcc
CFLAGS = -Wall -std=c99 -I./include -I./SDL2/include
LIBS = -L./SDL2/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

# Output directory for build artifacts
BUILD_DIR = build

# Platform-specific directory creation
ifeq ($(OS),Windows_NT)
    # Windows
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RMDIR = rmdir /S /Q $(BUILD_DIR)
else
    # Linux and macOS
    MKDIR = mkdir -p $(BUILD_DIR)
    RMDIR = rm -rf $(BUILD_DIR)
endif

SRC = src/*.c

# Main executable
EXECUTABLE = $(BUILD_DIR)/$(PROJECT_NAME)

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    # Windows
    EXECUTABLE := $(EXECUTABLE).exe
    LIBS += -lmingw32 -mwindows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux
        LIBS += -lm
    else ifeq ($(UNAME_S),Darwin)
        # macOS
        CFLAGS += -I/Library/Frameworks/SDL2.framework/Headers
        LIBS = -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_ttf
    endif
endif

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	$(MKDIR)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRC) $(LIBS)

clean:
	@echo "Cleaning up..."
	$(RMDIR)
