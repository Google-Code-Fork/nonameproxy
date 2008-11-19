# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic
CXX = g++

objects = info.o 

# to make plugins easy we always do a clean build

all: clean info

info: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o info.so $(objects)

info.o : info.cpp info.h 

clean:
	rm -f info.dll info.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O2 -I../..
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean info.dll

info.dll: CXXFLAGS += -DBUILD_DLL
info.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared -o info.dll $(objects) $(libs)

