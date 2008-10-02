# Makefile for nonameproxy

#CPPFLAGS = -Wall -02
CPPFLAGS = -Wall -g -O0
CC     = g++

objects = main.o connection.o connectionmanager.o corehooks.o gamestate.o loginstate.o \
		messagefactory.o messagelist.o networkmessage.o rsa.o server.o \
		tibiacrypt.o tibiamessage.o tibiatypes.o xtea.o client.o hookmanager.o \
		accountstate.o charstate.o safefile.o datreader.o mapstate.o adler32.o \
		idmanager.o messanger.o

all: nonameproxy

nonameproxy: $(objects)
	$(CC)  $(CPPFLAGS) -lgmp -o nonameproxy $(objects)

main.o : main.cpp connection.h connectionmanager.h corehooks.h gamestate.h loginstate.h \
		messagefactory.h messagelist.h networkmessage.h rsa.h server.h tibiacrypt.h \
		tibiamessage.h tibiatypes.h xtea.h

tibiamessage.o: tibiamessage.cpp tibiamessage.h tibiatypes.h enums.h

tibiatypes.o: tibiatypes.cpp tibiatypes.h networkmessage.h datreader.h enums.h

tibiacrypt.o: tibiacrypt.cpp tibiacrypt.h rsa.h xtea.h networkmessage.h adler32.h

rsa.o: rsa.cpp rsa.h

connectionmanager.o: connectionmanager.cpp connectionmanager.h connection.h

networkmessage.o: networkmessage.cpp networkmessage.h connection.h tibiacrypt.h tibiatypes.h

server.o: server.cpp server.h connection.h

connection.o: connection.cpp connection.h server.h

messagefactory.o: messagefactory.cpp messagefactory.cpp tibiamessage.h \
		tibiatypes.h networkmessage.h enums.h

corehooks.o: corehooks.cpp corehooks.h hook.h

loginstate.o: loginstate.cpp loginstate.h tibiatypes.h

gamestate.o: gamestate.cpp gamestate.h accountstate.h charstate.h

corehooks.o: corehooks.cpp corehooks.h hook.h tibiamessage.h

messagelist.o: messagelist.cpp messagelist.h 

xtea.o: xtea.cpp xtea.h

client.o: client.cpp client.h tibiacrypt.h connection.h networkmessage.h connectionmanager.h \
		tibiamessage.h hook.h corehooks.h hookmanager.h messagelist.h gamestate.h \
		loginstate.h

hookmanager.o: hookmanager.h hookmanager.cpp hook.h tibiamessage.h

accountstate.o: accountstate.h accountstate.cpp

charstate.o: charstate.h charstate.cpp enums.h

mapstate.o: mapstate.h mapstate.cpp

safefile.o: safefile.cpp safefile.h

datreader.o: datreader.cpp datreader.h safefile.h tibiatypes.h

adler32.o: adler32.cpp adler32.h

idmanager.o: idmanager.cpp idmanager.h

messanger.o: messanger.cpp messanger.h

clean:
	rm -f nonameproxy $(objects)
