# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = action.o 

# to make plugins easy we always do a clean build

all: clean action

action: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o action.so $(objects)

action.o : action.cpp action.h 

clean:
	rm -f action.dll action.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O2 -I../..
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean action.dll

action.dll: CXXFLAGS += -DBUILD_DLL
action.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared --enable-runtime-pseudo-reloc -o action.dll $(objects) $(libs)

