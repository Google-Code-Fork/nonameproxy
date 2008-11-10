# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CC     = g++

objects = watch.o 

# to make plugins easy we always do a clean build

all: clean watch

watch: $(objects)
	$(CC)  $(CPPFLAGS)  -shared -o watch.so $(objects)

watch.o : watch.cpp watch.h 

clean:
	rm -f watch.so $(objects)

