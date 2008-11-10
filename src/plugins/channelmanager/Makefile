# Makefile for nonameproxy

#CPPFLAGS = -Wall -02 -rdynamic
CPPFLAGS += -pg
CPPFLAGS += -I../.. -Wall -g -O0 -rdynamic
CXX     = g++
libs = 

#CPPFLAGS = -I../.. -Wall -O0 -g -static -LBUILD_DLL
#CXX=i586-mingw32msvc-g++
#LD=i586-mingw32msvc-ld
#AR=i586-mingw32msvc-ar
#RC=i586-mingw32msvc-windres
#libs = -lgmp -lws2_32

objects = channelmanager.o 

# to make plugins easy we always do a clean build

all: clean channelmanager

channelmanager: $(objects)
	$(CXX)  $(CPPFLAGS)  -shared -o channelmanager.so $(objects)

channelmanager.o : channelmanager.cpp channelmanager.h 

clean:
	rm -f channelmanager.so $(objects)
