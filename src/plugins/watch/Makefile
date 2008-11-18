# Makefile for nonameproxy

#CXXFLAGS = -Wall -02 -rdynamic
CXXFLAGS += -pg
CXXFLAGS += -I../.. -Wall -g -O0 -rdynamic
CXX = g++

objects = watch.o 

# to make plugins easy we always do a clean build

all: clean watch

watch: $(objects)
	$(CXX)  $(CXXFLAGS)  -shared -o watch.so $(objects)

watch.o : watch.cpp watch.h 

clean:
	rm -f watch.dll watch.so $(objects)

###################################################################
#WINDOWS STUFF
###################################################################
windows: CXXFLAGS = -Wall -O0 -I../..
windows: CXX=i586-mingw32msvc-g++
windows: LD=i586-mingw32msvc-ld
windows: AR=i586-mingw32msvc-ar
windows: RC=i586-mingw32msvc-windres
windows: libs=../../nonameproxy.dll

windows: clean watch.dll

watch.dll: CXXFLAGS += -DBUILD_DLL
watch.dll: $(objects)
	$(CXX) $(CXXFLAGS) -shared -o watch.dll $(objects) $(libs)

