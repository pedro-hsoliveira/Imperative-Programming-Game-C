GAME_NAME = theBendingOfAang

CC = gcc
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -g -Isrc -Iraylib/src

# recursively finds all .c files in src
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC_FILES = $(call rwildcard, src, *.c)

# object files go into build/ directory, mirroring src/ structure
OBJS = $(SRC_FILES:%.c=build/%.o)

# plataform-specific settings
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    # windows flags
    PLATFORM_OS = WINDOWS
    EXEC_EXT = .exe
    LDFLAGS = -Lraylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
else
    # linux flags
    PLATFORM_OS = LINUX
    EXEC_EXT = 
    LDFLAGS = -Lraylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

TARGET = build/$(GAME_NAME)$(EXEC_EXT)

.PHONY: all clean run raylib_build

all: raylib_build $(TARGET)

raylib_build:
	$(MAKE) -C raylib/src

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf build
	$(MAKE) -C raylib/src clean