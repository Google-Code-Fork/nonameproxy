# Makefile for nonameproxy

CFLAGS  = -Wall
CFLAGS  += -lgmp
CFLAGS += -g
#CFLAGS += -O2
CC     = g++

objects = main.o connection.o connectionmanager.o corehooks.o gamestate.o loginstate.o \
		messagefactory.o messagelist.o networkmessage.o rsa.o server.o \
		tibiacrypt.o tibiamessage.o tibiatypes.o xtea.o

all: nonameproxy

nonameproxy: $(objects)
	$(CC) $(CFLAGS) -o nonameproxy $(objects)

main.o : main.cpp connection.h connectionmanager.h corehooks.h gamestate.h loginstate.h \
		messagefactory.h messagelist.h networkmessage.h rsa.h server.h tibiacrypt.h \
		tibiamessage.h tibiatypes.h xtea.h

tibiamessage.o: tibiamessage.cpp tibiamessage.h tibiatypes.h

tibiatypes.o: tibiatypes.cpp tibiatypes.h networkmessage.h

tibiacrypt.o: tibiacrypt.cpp tibiacrypt.h rsa.h xtea.h networkmessage.h

rsa.o: rsa.cpp rsa.h

connectionmanager.o: connectionmanager.cpp connectionmanager.h connection.h

networkmessage.o: networkmessage.cpp networkmessage.h connection.h tibiacrypt.h tibiatypes.h

server.o: server.cpp server.h connection.h

connection.o: connection.cpp connection.h server.h

messagefactory.o: messagefactory.cpp messagefactory.cpp tibiamessage.h tibiatypes.h networkmessage.h

corehooks.o: corehooks.cpp corehooks.h hook.h

loginstate.o: loginstate.cpp loginstate.h tibiatypes.h

gamestate.o: gamestate.cpp gamestate.h

corehooks.o: corehooks.cpp corehooks.h hook.h tibiamessage.h

messagelist.o: messagelist.cpp messagelist.h 

xtea.o: xtea.cpp xtea.h

clean:
	rm -f nonameproxy $(objects)
