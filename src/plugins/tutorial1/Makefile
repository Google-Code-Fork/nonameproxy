# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = tutorial1.o 

# to make plugins easy we always do a clean build

all: clean tutorial1

tutorial1: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o tutorial1.so $(objects)

tutorial1.o : tutorial1.cpp tutorial1.h 

clean:
	rm -f tutorial1.dll tutorial1.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O0 -I../..

windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres

#windows: CXX=/home/carl/mingw/bin/i386-mingw32-g++
#windows: LD=/home/carl/mingw/bin/i386-mingw32-ld
#windows: AR=/home/carl/mingw/bin/i386-mingw32-ar
#windows: RC=/home/carl/mingw/bin/i386-mingw32-windres

windows: libs=../../nonameproxy.dll

windows: clean tutorial1.dll

tutorial1.dll: CXXFLAGS += -DBUILD_DLL
tutorial1.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared -o tutorial1.dll $(objects) $(libs)

