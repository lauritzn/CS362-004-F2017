CFLAGS = -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

randomtestcard1: randomtestcard1.c dominion.o
	
	gcc -o randomtestcard1 dominion.c rngs.c randomtestcard1.c $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o
	
	gcc -o randomtestcard2 dominion.c rngs.c randomtestcard2.c $(CFLAGS) 

randomtestadventurer: randomtestadventurer.c dominion.o
	
	gcc -o randomtestadventurer dominion.c rngs.c randomtestadventurer.c $(CFLAGS)

random: randomtestcard1.c randomtestcard2.c randomtestadventurer.c dominion.o
	
	echo "randomtestcard1.c:" >> randomtestcard1.out
	gcc -o randomtestcard1 dominion.c rngs.c randomtestcard1.c $(CFLAGS)
	randomtestcard1 >> randomtestcard1.out
	gcov dominion.c -b >> randomtestcard1.out 
	echo "randomtestcard2.c:" >> randomtestcard2.out
	gcc -o randomtestcard2 dominion.c rngs.c randomtestcard2.c $(CFLAGS)
	randomtestcard2 >> randomtestcard2.out
	gcov dominion.c -b >> randomtestcard2.out 
	echo "randomtestadventurer.c:" >> randomtestadventurer.out
	gcc -o randomtestadventurer dominion.c rngs.c randomtestadventurer.c $(CFLAGS)
	randomtestadventurer >> randomtestadventurer.out
	gcov dominion.c -b >> randomtestadventurer.out 

unittestresults.out: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.o
	
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 dominion.c rngs.c unittest1.c $(CFLAGS)
	unittest1 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 
	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 dominion.c rngs.c unittest2.c $(CFLAGS)
	unittest2 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 
	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 dominion.c rngs.c unittest3.c $(CFLAGS)
	unittest3 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 
	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 dominion.c rngs.c unittest4.c $(CFLAGS)
	unittest4 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 
	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 dominion.c rngs.c cardtest1.c $(CFLAGS)
	cardtest1 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 
	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 dominion.c rngs.c cardtest2.c $(CFLAGS)
	cardtest2 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 
	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 dominion.c rngs.c cardtest3.c $(CFLAGS)
	cardtest3 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 
	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 dominion.c rngs.c cardtest4.c $(CFLAGS)
	cardtest4 >> unittestresults.out
	gcov dominion.c -b >> unittestresults.out 

unittest1: unittest1.c dominion.o
	
	gcc -o unittest1 dominion.c rngs.c unittest1.c $(CFLAGS)

unittest2: unittest2.c dominion.o
	
	gcc -o unittest2 dominion.c rngs.c unittest2.c $(CFLAGS)
	
unittest3: unittest3.c dominion.o
	
	gcc -o unittest3 dominion.c rngs.c unittest3.c $(CFLAGS)
	
unittest4: unittest4.c dominion.o
	
	gcc -o unittest4 dominion.c rngs.c unittest4.c $(CFLAGS)
	
cardtest1: cardtest1.c dominion.o
	
	gcc -o cardtest1 dominion.c rngs.c cardtest1.c $(CFLAGS)

cardtest2: cardtest2.c dominion.o
	
	gcc -o cardtest2 dominion.c rngs.c cardtest2.c $(CFLAGS)

cardtest3: cardtest3.c dominion.o
	
	gcc -o cardtest3 dominion.c rngs.c cardtest3.c $(CFLAGS)

cardtest4: cardtest4.c dominion.o
	
	gcc -o cardtest4 dominion.c rngs.c cardtest4.c $(CFLAGS)
	
all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe randomtestcard1 randomtestcard2 randomtestadventurer unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
