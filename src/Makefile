# Makefile for nonameproxy

#Linux
#CPPFLAGS += -Wall -02 -rdynamic
#CPPFLAGS += -DDEBUG_HOOK
CPPFLAGS += -pg
CPPFLAGS += -Wall -g -O0 -rdynamic
libs = -ldl -lgmp
out = nonameproxy

CXX     = g++

#WIN32
#CPPFLAGS = -Wall -O0 -g -static
#CXX=i586-mingw32msvc-g++
#LD=i586-mingw32msvc-ld
#AR=i586-mingw32msvc-ar
#RC=i586-mingw32msvc-windres
#libs = -lgmp -lws2_32
#out = nonameproxy.exe

objects = main.o connection.o connectionmanager.o corehooks.o gamestate.o loginstate.o \
		messagefactory.o messagelist.o networkmessage.o rsa.o server.o \
		tibiacrypt.o tibiamessage.o tibiatypes.o xtea.o client.o hookmanager.o \
		accountstate.o charstate.o safefile.o datreader.o mapstate.o adler32.o \
		idmanager.o messenger.o plugin.o pluginmanager.o timer.o corerecipricant.o \
		pos.o thing.o packethookmanager.o typeparser.o inventorystate.o

sharedobjects = channelmanager.so console.so debug.so info.so autogroup.so test.so watch.so

all: nonameproxy

nonameproxy: $(objects)
	$(CXX)  $(CPPFLAGS) -o $(out) $(objects) $(libs)

main.o : main.cpp connection.h connectionmanager.h corehooks.h gamestate.h loginstate.h \
		messagefactory.h messagelist.h networkmessage.h rsa.h server.h tibiacrypt.h \
		tibiamessage.h tibiatypes.h xtea.h

tibiamessage.o: tibiamessage.cpp tibiamessage.h tibiatypes.h enums.h

tibiatypes.o: tibiatypes.cpp tibiatypes.h networkmessage.h datreader.h enums.h timer.h

tibiacrypt.o: tibiacrypt.cpp tibiacrypt.h rsa.h xtea.h networkmessage.h adler32.h

rsa.o: rsa.cpp rsa.h

connectionmanager.o: connectionmanager.cpp connectionmanager.h connection.h idmanager.h

networkmessage.o: networkmessage.cpp networkmessage.h connection.h tibiacrypt.h tibiatypes.h \
			timer.h

server.o: server.cpp server.h connection.h

connection.o: connection.cpp connection.h server.h

messagefactory.o: messagefactory.cpp messagefactory.cpp tibiamessage.h \
		tibiatypes.h networkmessage.h enums.h

corehooks.o: corehooks.cpp corehooks.h hook.h

loginstate.o: loginstate.cpp loginstate.h tibiatypes.h

gamestate.o: gamestate.cpp gamestate.h accountstate.h charstate.h mapstate.h inventorystate.h

corehooks.o: corehooks.cpp corehooks.h hook.h tibiamessage.h gamestate.h client.h

messagelist.o: messagelist.cpp messagelist.h 

xtea.o: xtea.cpp xtea.h

client.o: client.cpp client.h tibiacrypt.h connection.h networkmessage.h connectionmanager.h \
		tibiamessage.h hook.h corehooks.h hookmanager.h messagelist.h gamestate.h \
		loginstate.h rsakeys.h messagelist.h enums.h datreader.h messenger.h \
		corerecipricant.h packethookmanager.h

hookmanager.o: hookmanager.h hookmanager.cpp hook.h tibiamessage.h

accountstate.o: accountstate.h accountstate.cpp

charstate.o: charstate.h charstate.cpp enums.h

mapstate.o: mapstate.h mapstate.cpp pos.h thing.h typeparser.h enums.h

inventorystate.o: inventorystate.h inventorystate.cpp pos.h thing.h typeparser.h enums.h

safefile.o: safefile.cpp safefile.h

datreader.o: datreader.cpp datreader.h safefile.h tibiatypes.h

adler32.o: adler32.cpp adler32.h

idmanager.o: idmanager.cpp idmanager.h

messenger.o: messenger.cpp messenger.h

plugin.o: plugin.h plugin.cpp 

pluginmanager.o: pluginmanager.cpp pluginmanager.h plugin.h idmanager.h hookmanager.h messenger.h \
		connectionmanager.h packethookmanager.h

timer.o: timer.cpp timer.h

corerecipricant.o: corerecipricant.h corerecipricant.cpp client.h

pos.o: pos.cpp pos.h

thing.o: thing.cpp thing.h

packethookmanager.o: packethookmanager.cpp packethookmanager.h packethook.h networkmessage.h

typeparser.o: typeparser.cpp typeparser.h gamestate.h tibiatypes.h thing.h

clean:
	rm -f nonameproxy $(objects)

plugins: $(sharedobjects)

channelmanager.so:
	make -C ./plugins/channelmanager/
	
console.so:
	make -C ./plugins/console/
	
debug.so:
	make -C ./plugins/debug/
	
info.so:
	make -C ./plugins/info/
	
autogroup.so:
	make -C ./plugins/autogroup/
	
test.so:
	make -C ./plugins/test/
	
watch.so:
	make -C ./plugins/watch/
	
