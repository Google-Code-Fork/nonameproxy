# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CC     = g++

objects = channelmanager.o 

# to make plugins easy we always do a clean build

all: clean channelmanager

channelmanager: $(objects)
	$(CC)  $(CPPFLAGS)  -shared -o channelmanager.so $(objects)

channelmanager.o : channelmanager.cpp channelmanager.h 

clean:
	rm -f channelmanager.so $(objects)
