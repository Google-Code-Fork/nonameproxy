# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = autogroup.o 

# to make plugins easy we always do a clean build

all: clean autogroup

autogroup: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o autogroup.so $(objects)

autogroup.o : autogroup.cpp autogroup.h 

clean:
	rm -f autogroup.dll autogroup.so $(objects)

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

windows: clean autogroup.dll

autogroup.dll: CXXFLAGS += -DBUILD_DLL
autogroup.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared --enable-runtime-pseudo-reloc -o autogroup.dll $(objects) $(libs)
