# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic
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
windows: CXXFLAGS = -Wall -O0 -I../..
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean console.dll

console.dll: CXXFLAGS += -DBUILD_DLL
console.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared -o console.dll $(objects) $(libs)

