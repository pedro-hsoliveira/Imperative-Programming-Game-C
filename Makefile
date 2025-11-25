# general parameters
CFLAGS = -Wall -Wextra -g
LIBS = -lraylib -lm -lpthread -ldl
SOURCES = src/main.c \
	 $(wildcard src/entities/*.c) \
	 $(wildcard src/screens/*.c) \
	 $(wildcard src/gameplay/game.c) \
	 $(wildcard src/gameplay/map/*.c)

# linux parameters
CC_LINUX = gcc
LIBS_LINUX = -lraylib -lm -lpthread -ldl 
OUT_LINUX = build/theBendingOfAang

# windows parameters
CC_WIN = x86_64-w64-mingw32-gcc
FLAGS_WIN = -Iraylib/include -Lraylib/lib
LIBS_WIN = -lraylib -lopengl32 -lgdi32 -lwinmm -static
OUT_WIN = build/theBendingOfAang.exe

# makefile will, by default, create both executables
all: linux windows

linux: $(SOURCES)
	# creates the build directory if it doesn't exists
	mkdir -p build
	$(CC_LINUX) $(CFLAGS) $(SOURCES) -o $(OUT_LINUX) $(LIBS_LINUX)

windows: $(SOURCES)
	# same thing siad above
	mkdir -p build
	$(CC_WIN) $(CFLAGS) $(SOURCES) -o $(OUT_WIN) $(FLAGS_WIN) $(LIBS_WIN)

clean:
	rm -f build/theBendingOfAang build/theBendingOfAang.exe
