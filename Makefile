GAME_NAME = theBendingOfAang

CC = gcc
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -g -Isrc -Iraylib/src

# Recursively finds all .c files in src
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_FILES = $(call rwildcard, src, *.c)

# Object files go into build/ directory
OBJS = $(SRC_FILES:%.c=build/%.o)

# Detect Windows vs Linux
ifeq ($(OS),Windows_NT)
    PLATFORM_OS = WINDOWS
    EXEC_EXT = .exe
    LDFLAGS = -Lraylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
    # Comando para criar pastas no Windows (substitui mkdir -p)
    MKDIR_CMD = if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
    # Comando para limpar no Windows
    CLEAN_CMD = if exist build rmdir /s /q build
else
    PLATFORM_OS = LINUX
    EXEC_EXT = 
    LDFLAGS = -Lraylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    MKDIR_CMD = mkdir -p $(dir $@)
    CLEAN_CMD = rm -rf build
endif

TARGET = build/$(GAME_NAME)$(EXEC_EXT)

.PHONY: all clean run raylib_build

all: raylib_build $(TARGET)

raylib_build:
	$(MAKE) -C raylib/src

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

build/%.o: %.c
	$(MKDIR_CMD)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(CLEAN_CMD)
	$(MAKE) -C raylib/src clean