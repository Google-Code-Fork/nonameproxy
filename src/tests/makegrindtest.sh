#!/bin/bash
g++ -I ../ grind.cpp ../networkmessage.o ../tibiamessage.o ../messagelist.o ../messagefactory.o ../tibiatypes.o ../datreader.o ../safefile.o ../gamestate.o ../mapstate.o -Wall -fno-access-control -o grind 
