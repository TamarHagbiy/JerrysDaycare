JerryBoree: JerryBoreeMain.o HashTable.o Jerry.o KeyValuePair.o LinkedList.o MultiValueHashTable.o
	gcc JerryBoreeMain.o HashTable.o Jerry.o KeyValuePair.o LinkedList.o MultiValueHashTable.o -o JerryBoree

JerryBoreeMain.o: JerryBoreeMain.c LinkedList.h MultiValueHashTable.h Jerry.h HashTable.h Defs.h
	gcc -c JerryBoreeMain.c

HashTable.o: HashTable.c HashTable.h LinkedList.h Defs.h
	gcc -c HashTable.c

Jerry.o: Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c

KeyValuePair.o: KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c

LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c

MultiValueHashTable.o: MultiValueHashTable.c MultiValueHashTable.h HashTable.h Defs.h LinkedList.h
	gcc -c MultiValueHashTable.c

clean:
	rm -f *.o JerryBoree
