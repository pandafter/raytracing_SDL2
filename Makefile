CC = gcc
CFLAGS = -O3 -Wall -Wextra -Iinclude -D_REENTRANT
LDFLAGS = -lSDL2 -lm
SRC = src/main.c src/render.c src/physics.c src/animation.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = raytracing

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) *.d