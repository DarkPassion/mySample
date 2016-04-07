
rm -f data.o
bjcopy -I binary -O elf64-x86-64 --binary-architecture i386 data.txt data.o

nm data.o

rm -f test.exe

gcc -o test.exe main.c data.o


