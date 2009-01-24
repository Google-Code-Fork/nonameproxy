# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = debug.o 

# to make plugins easy we always do a clean build

all: clean debug

debug: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o debug.so $(objects)

debug.o : debug.cpp debug.h 

clean:
	rm -f debug.dll debug.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
#windows: CXXFLAGS = -Wall -O2 -I../..
windows: CXXFLAGS = -Wall -O0 -I../.. -g
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean debug.dll

debug.dll: CXXFLAGS += -DBUILD_DLL
debug.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared --enable-runtime-pseudo-reloc -o debug.dll $(objects) $(libs)

