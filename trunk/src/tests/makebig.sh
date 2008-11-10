#!/bin/bash
g++ packettest.cpp networkmessage.o tibiamessage.o messagelist.o messagefactory.o tibiatypes.o datreader.o safefile.o -Wall -fno-access-control -o bigpackettest
