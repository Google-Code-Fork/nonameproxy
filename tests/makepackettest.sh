#!/bin/bash
g++ -I ../ packettest.cpp ../networkmessage.o ../tibiamessage.o ../messagelist.o ../messagefactory.o ../tibiatypes.o ../datreader.o ../timer.o ../safefile.o ../gamestate.o ../mapstate.o -Wall -fno-access-control -o packettest
