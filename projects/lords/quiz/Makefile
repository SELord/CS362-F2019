CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc  -o testme -g  testme.c $(CFLAGS)
	./testme &> testme.out
	echo ________________________________________________________________________________________________________________________________________________________ >> testme.out
	gcov testme.c -b >> testme.out
	echo ________________________________________________________________________________________________________________________________________________________ >> testme.out
	cat testme.c.gcov >> testme.out



all: testme

clean:
	rm -f *.o testme *.gcov *.gcda *.gcno *.so *.out 
	rm -r *.dSYM