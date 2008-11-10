# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I.. -Wall -g -O0 -rdynamic
CC     = g++

# to make plugins easy we always do a clean build

all: clean tiletest containertest

tiletest:
	$(CC)  $(CPPFLAGS) -fno-access-control -o tiletest tiletest.cpp ../datreader.o \
	../thing.o ../mapstate.o ../safefile.o ../pos.o

containertest:
	$(CC)  $(CPPFLAGS) -fno-access-control -o containertest containertest.cpp \
	../thing.o ../inventorystate.o 


clean:
	rm -f tiletest containertest

