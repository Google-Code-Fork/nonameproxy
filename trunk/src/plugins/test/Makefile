# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic
CXX = g++

objects = test.o 

# to make plugins easy we always do a clean build

all: clean test

test: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o test.so $(objects)

test.o : test.cpp test.h 

clean:
	rm -f test.dll test.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O0 -I../..
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean test.dll

test.dll: CXXFLAGS += -DBUILD_DLL
test.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared -o test.dll $(objects) $(libs)

