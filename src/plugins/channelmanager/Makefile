# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = channelmanager.o 

# to make plugins easy we always do a clean build

all: clean channelmanager

channelmanager: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o channelmanager.so $(objects)

channelmanager.o : channelmanager.cpp channelmanager.h 

clean:
	rm -f channelmanager.dll channelmanager.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O2 -I../..

windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres

#windows: CXX=/home/carl/mingw/bin/i386-mingw32-g++
#windows: LD=/home/carl/mingw/bin/i386-mingw32-ld
#windows: AR=/home/carl/mingw/bin/i386-mingw32-ar
#windows: RC=/home/carl/mingw/bin/i386-mingw32-windres

windows: libs=../../nonameproxy.dll

windows: clean channelmanager.dll

channelmanager.dll: CXXFLAGS += -DBUILD_DLL
channelmanager.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared --enable-runtime-pseudo-reloc -o channelmanager.dll $(objects) $(libs)

