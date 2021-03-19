# Makefile for Distance Vector Algorithm implementation
# credit: https://www.softwaretestinghelp.com/cpp-makefile-tutorial/
 
# *****************************************************
# Variables to control Makefile operation
 
CC = c++
CFLAGS = -std=c++11 -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o router.o network.o
	$(CC) $(CFLAGS) -o main main.o router.o network.o
 
# The main.o target can be written more simply
 
main.o: main.cpp router.hpp network.hpp
	$(CC) $(CFLAGS) -c main.cpp
 
router.o: router.hpp
 
network.o: router.hpp network.hpp