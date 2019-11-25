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

unittest2: unittest2.c dominion.o noAbortAssert.o rngs.o
	gcc  -o unittest2 -g  unittest2.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o noAbortAssert.o rngs.o
	gcc  -o unittest3 -g  unittest3.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o noAbortAssert.o rngs.o
	gcc  -o unittest4 -g  unittest4.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

unittest5: unittest5.c dominion.o noAbortAssert.o rngs.o
	gcc  -o unittest5 -g  unittest5.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

unittestresults: unittest1 unittest2 unittest3 unittest4 unittest5
	echo _____________________________________________________________________________________________________________________________________ unittest1 - baron > unittestresults.out
	echo ________________________________________________________________________________________________________________________________________________________ >> unittestresults.out
	./unittest1 >> unittestresults.out
	echo _____________________________________________________________________________________________________________________________________ unittest2 - minion >> unittestresults.out
	echo ________________________________________________________________________________________________________________________________________________________ >> unittestresults.out
	./unittest2 >> unittestresults.out
	echo _____________________________________________________________________________________________________________________________________ unittest3 - ambassador >> unittestresults.out
	echo _____________________________________________________________________________________________________________________________________________________________ >> unittestresults.out
	./unittest3 >> unittestresults.out
	echo _____________________________________________________________________________________________________________________________________ unittest4 - tribute >> unittestresults.out
	echo _________________________________________________________________________________________________________________________________________________________ >> unittestresults.out
	./unittest4 >> unittestresults.out
	echo _____________________________________________________________________________________________________________________________________ unittest5 - mine >> unittestresults.out
	echo ______________________________________________________________________________________________________________________________________________________ >> unittestresults.out
	./unittest5 >> unittestresults.out
	echo _____________________________________________________________________________________________________________________________________ Code Coverage Results >> unittestresults.out
	echo ___________________________________________________________________________________________________________________________________________________________ >> unittestresults.out
	gcov dominion.c -fb >> unittestresults.out
	echo _____________________________________________________________________________________________________________________________________ Contents of dominion.c.gcov >> unittestresults.out
	echo _________________________________________________________________________________________________________________________________________________________________ >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out


randomtestcard1: randomtestcard1.c dominion.o noAbortAssert.o rngs.o
	gcc  -o randomtestcard1 -g  randomtestcard1.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o noAbortAssert.o rngs.o
	gcc  -o randomtestcard2 -g  randomtestcard2.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

randomtestcard3: randomtestcard3.c dominion.o noAbortAssert.o rngs.o
	gcc  -o randomtestcard3 -g  randomtestcard3.c dominion.o noAbortAssert.o rngs.o $(CFLAGS)

randomtestresults: randomtestcard1 randomtestcard2 randomtestcard3
	echo _____________________________________________________________________________________________________________________________________ randomtestcard1 - baron > randomtestresults.out
	echo ________________________________________________________________________________________________________________________________________________________ >> randomtestresults.out
	./randomtestcard1 >> randomtestresults.out 2>&1
	echo _____________________________________________________________________________________________________________________________________ randomtestcard2 - minion >> randomtestresults.out
	echo ________________________________________________________________________________________________________________________________________________________ >> randomtestresults.out
	./randomtestcard2 >> randomtestresults.out 2>&1
	echo _____________________________________________________________________________________________________________________________________ randomtestcard3 - tribute >> randomtestresults.out
	echo _________________________________________________________________________________________________________________________________________________________ >> randomtestresults.out
	./randomtestcard3 >> randomtestresults.out 2>&1
	echo _____________________________________________________________________________________________________________________________________ Code Coverage Results >> randomtestresults.out
	echo ___________________________________________________________________________________________________________________________________________________________ >> randomtestresults.out
	gcov dominion.c -fb >> randomtestresults.out 2>&1
	echo _____________________________________________________________________________________________________________________________________ Contents of dominion.c.gcov >> randomtestresults.out
	echo _________________________________________________________________________________________________________________________________________________________________ >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out 2>&1


alltests: unittestresults randomtestresults



interface.o: interface.c interface.h
	gcc -c interface.c -g  $(CFLAGS)

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom unittestresults player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out unittest1 unittest2 unittest3 unittest4 unittest5 unittestresults unittestresults.out randomtestcard1 randomtestcard2 randomtestcard3
	rm -r *.dSYM
