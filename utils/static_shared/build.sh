#!/bin/bash


g++ -c static.cc 
ar -r libstatic.a static.o

rm -f static.o

g++ -shared -fPIC -o libshared.so shared.cc


g++ main.cc libstatic.a -L. -lshared -Wl,-rpath,. -o main.out



