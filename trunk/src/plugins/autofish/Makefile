# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic -fPIC
CXX = g++

objects = autofish.o 

# to make plugins easy we always do a clean build

all: clean autofish

autofish: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o autofish.so $(objects)

autofish.o : autofish.cpp autofish.h 

clean:
	rm -f autofish.dll autofish.so $(objects)

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

windows: clean autofish.dll

autofish.dll: CXXFLAGS += -DBUILD_DLL
autofish.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared -o autofish.dll $(objects) $(libs)

