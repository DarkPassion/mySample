

CC=g++
AR=ar

RM=rm -f

default: all

all:
	@(echo hellowold)
	@($(CC) main.cc Buffer.cc -Wl,-rpath,. -o main.out)

clean:
	@($(RM) *.a *.so *.o *.out)


