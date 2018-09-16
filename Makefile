all: prog

prog: main.o pathenumeration.o llist.o mynode.o
	g++ main.o pathenumeration.o llist.o mynode.o -o prog

llist.o: mynode.h llist.cpp llist.h
	g++ -O2 llist.cpp -lemon

pathenumeration.o: mynode.h  llist.h pathenumeration.cpp pathenumeration.h
	g++ -O2 pathenumeration.cpp  -lemon

main.o: mynode.h llist.h pathenumeration.h main.cpp
	g++ -O2 main.cpp -lemon

clean:
	rm *.o output