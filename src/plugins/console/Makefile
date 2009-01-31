# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = console.o 

# to make plugins easy we always do a clean build

all: clean console

console: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o console.so $(objects)

console.o : console.cpp console.h 

clean:
	rm -f console.dll console.so $(objects)

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

windows: clean console.dll

console.dll: CXXFLAGS += -DBUILD_DLL
console.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared --enable-runtime-pseudo-reloc -o console.dll $(objects) $(libs)

