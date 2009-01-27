# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = map.o localmap.o globalmap.o mapgraph.o astar.o

# to make plugins easy we always do a clean build

all: clean map

tests: $(objects) graphtest

graphtest: mapgraphtest.cpp mapgraph.h astar.h mapgraph.o astar.o
	$(CXX) $(CXXFLAGS) -o graphtest mapgraphtest.cpp mapgraph.o astar.o ../../pos.o
	
map: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o map.so $(objects)

map.o : map.cpp map.h 

astar.o: astar.cpp astar.h

localmap.o : localmap.cpp localmap.h map.h

globalmap.o : globalmap.cpp globalmap.h map.h mapgraph.h

mapgraph.o : mapgraph.cpp mapgraph.h astar.h


clean:
	rm -f map.dll map.so graphtest $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O2 -I../..
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean map.dll

map.dll: CXXFLAGS += -DBUILD_DLL
map.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared --enable-runtime-pseudo-reloc -o map.dll $(objects) $(libs)

