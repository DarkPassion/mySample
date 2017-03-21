

CC = g++
RM = rm -f

default: all

all:
	@(echo build test fork)
	@($(CC) test_fork.cc -L. -lpthread -Wall -Wl,-rpath,. -o test_fork.out)

clean:
	@($(RM) *.a *.so *.o *.out)


