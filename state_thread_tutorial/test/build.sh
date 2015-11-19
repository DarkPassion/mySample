#!/bin/bash

#CC=/usr/bin/g++


g++ -I../st-1.9/obj main.cpp -I. st_help.cpp st_thread.cpp st_udp.cpp st_tcp.cpp st_msg.cpp -L../st-1.9/obj/ -lst -o testlibst.out
