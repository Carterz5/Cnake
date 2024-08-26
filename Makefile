# Define compiler and flags
CC = gcc
CFLAGS = -Wall -g -I/usr/include/freetype2 -I/usr/include/libpng16
LDFLAGS = -lGLEW -lglfw -lGL -lGLU -lX11 -lfreetype

# Define source files and output
SRCS = snake.c graphics.c gameplay.c
OBJS = $(SRCS:.c=.o)
TARGET = Snake.out

# Default rule to build the program
all: $(TARGET)

# Rule to link the object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
