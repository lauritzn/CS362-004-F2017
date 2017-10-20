CFLAGS = -fpic -coverage -lm

testme: testme.c
	gcc -o testme testme.c $(CFLAGS)
	testme

all: testme

default: testme
	 
clean:
	 rm -f testme *.o *.gcov *.gcda *.gcno *.so *.out
	
