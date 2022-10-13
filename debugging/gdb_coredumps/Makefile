# Copyright (c) 2022, Piotr Staniszewski

CXX = g++
CFLAGS = -Wall -Wextra -pedantic -o2 -g

TARGET_LIB = libbestiary.so
TARGET_APP = game

all: ${TARGET_LIB} ${TARGET_APP}

dbg: game.dbg bestiary.dbg

${TARGET_LIB}: bestiary.o
	$(CXX) -shared bestiary.o -o ${TARGET_LIB}

bestiary.o: bestiary.cpp bestiary.h
	$(CXX) $(CFLAGS) -fPIC -c bestiary.cpp -o bestiary.o

${TARGET_APP}: game.o
	$(CXX) game.o -L. -lbestiary -o ${TARGET_APP}

game.o: game.cpp
	$(CXX) $(CFLAGS) -c game.cpp -o game.o

game.dbg: ${TARGET_APP}
	objcopy --only-keep-debug ${TARGET_APP} ${TARGET_APP}.dbg
	strip --strip-debug --strip-unneeded ${TARGET_APP} 

bestiary.dbg: ${TARGET_LIB}
	objcopy --only-keep-debug ${TARGET_LIB} ${TARGET_LIB}.dbg
	strip --strip-debug --strip-unneeded ${TARGET_LIB}

clean:
	$(RM) *.o *.so ${TARGET_APP} *.dbg core*
