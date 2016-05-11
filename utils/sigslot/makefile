

CC = gcc

RM = rm -f

CFLAGS = -I. -Wall -lstdc++

DEPS = sigslot.h

OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


sigslot: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OBJ) sigslot

