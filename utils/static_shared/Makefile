

CC=g++
AR=ar

RM=rm -f

default: all

all:
	@(echo hellowold)
	@($(CC) -c static.cc )
	@($(AR) -r libstatic.a static.o; rm -f static.o)
	@($(CC) -shared -fPIC shared.cc -o libshared.so)
	@($(CC) main.cc -L. -lshared -lstatic -Wl,-rpath,. -o main.out)
	@($(CC) main.cc libstatic.a -L. -lshared -Wl,-rpath,. -o main2.out)


clean:
	@($(RM) *.a *.so *.o *.out)


