# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic
CXX = g++

objects = map.o 

# to make plugins easy we always do a clean build

all: clean map

map: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o map.so $(objects)

map.o : map.cpp map.h 

clean:
	rm -f map.dll map.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O0 -I../..
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean map.dll

map.dll: CXXFLAGS += -DBUILD_DLL
map.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared -o map.dll $(objects) $(libs)

