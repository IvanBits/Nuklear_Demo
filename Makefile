CC = gcc
CFLAGS = -Wall -Wextra -I./include -I./libs/glew/include -I./libs/glfw/include -DGLEW_STATIC
LDFLAGS = -L./libs/glfw/lib-mingw-w64 -L./libs/glew/lib/Release/x64 -static -mwindows
LDLIBS = -lglew32s -lglfw3 -lopengl32 -lgdi32 -lkernel32 -luser32 -lshell32

SRC = src/main.c
OBJ = $(SRC:.c=.o)
TARGET = calculator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q /F src\*.o $(TARGET).exe

.PHONY: all clean