CC = clang
CFLAGS = -Wall -Wextra -Iinclude -g

SRC = src/hash.c src/main.c
OBJ = $(SRC:.c=.o)
EXEC = hash_function.exe

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /F /Q $(EXEC) 2>nul || echo No files to clean