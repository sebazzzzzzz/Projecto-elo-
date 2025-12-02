CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/cpu.c \
      src/proceso.c \
      src/cola_prioridad.c \
      src/maxheap.c \
      src/lista.c \
      src/hash.c \
      src/reportes.c

OBJ = $(SRC:.c=.o)

all: cpu

cpu: $(OBJ)
	$(CC) $(CFLAGS) -o cpu $(OBJ)

clean:
	rm -f $(OBJ) cpu
