# MAKEFILE for nonameproxy

CC=g++
CFLAGS=-Wall
LDFLAGS=-lgmp

all: nonameproxy

nonameproxy: main.cpp messagefactory.o tibiamessage.o tibiatypes.o tibiacrypt.o xtea.o rsa.o connectionmanager.o connection.o server.o networkmessage.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o nonameproxy main.cpp messagefactory.o tibiamessage.o tibiatypes.o tibiacrypt.o xtea.o rsa.o connectionmanager.o connection.o server.o networkmessage.o


tibiamessage.o: tibiamessage.cpp tibiamessage.h tibiatypes.h
	$(CC) -c $(CFLAGS) $(CFLAGS) tibiamessage.cpp

tibiatypes.o: tibiatypes.cpp tibiatypes.h networkmessage.h
	$(CC) -c $(CFLAGS) $(CFLAGS) tibiatypes.cpp

tibiacrypt.o: tibiacrypt.cpp tibiacrypt.h rsa.h xtea.h networkmessage.h
	$(CC) -c $(CFLAGS) $(CFLAGS) tibiacrypt.cpp

xtea.o: xtea.cpp xtea.h
	$(CC) -c $(CFLAGS) $(CFLAGS) xtea.cpp
	
rsa.o: rsa.cpp rsa.h
	$(CC) -c $(CFLAGS) $(CFLAGS) rsa.cpp

connectionmanager.o: connectionmanager.cpp connectionmanager.h connection.h
	$(CC) -c $(CFLAGS) $(CFLAGS) connectionmanager.cpp

networkmessage.o: networkmessage.cpp networkmessage.h connection.h tibiacrypt.h tibiatypes.h
	$(CC) -c $(CFLAGS) $(CFLAGS) networkmessage.cpp

server.o: server.cpp server.h connection.h
	$(CC) -c $(CFLAGS) $(CFLAGS) server.cpp

connection.o: connection.cpp connection.h server.h
	$(CC) -c $(CFLAGS) $(CFLAGS) connection.cpp

messagefactory.o: messagefactory.cpp messagefactory.cpp tibiamessage.h tibiatypes.h networkmessage.h
	$(CC) -c $(CFLAGS) $(CFLAGS) messagefactory.cpp

clean:
	rm -f nonameproxy messagefactory.o tibiamessage.o tibiatypes.o tibiacrypt.o xtea.o rsa.o connectionmanager.o connection.o server.o networkmessage.o
