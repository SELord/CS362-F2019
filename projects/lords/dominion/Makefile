CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.c rngs.h
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.c dominion.h rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: playdom.c dominion.o
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

noAbortAssert.o: noAbortAssert.c noAbortAssert.h
	gcc -c noAbortAssert.c -g $(CFLAGS)

unittest1: unittest1.c dominion.o noAbortAssert.o rngs.o
	gcc  -o unittest1 -g  unittest1.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

interface.o: interface.c interface.h
	gcc -c interface.c -g  $(CFLAGS)

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom unittest1 player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out unittest1
	rm -r playdom.dSYM player.dSYM unittest1.dSYM
