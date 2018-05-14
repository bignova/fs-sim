#
# Macro definitions
#
CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wshadow -Wwrite-strings

all: public01.x public02.x public03.x public04.x public05.x public06.x \
     public07.x public08.x public09.x public10.x

public01.x: public01.o fs-sim.o
	$(CC) public01.o fs-sim.o -o public01.x

public02.x: public02.o fs-sim.o
	$(CC) public02.o fs-sim.o -o public02.x

public03.x: public03.o fs-sim.o memory-checking.o
	$(CC) public03.o fs-sim.o memory-checking.o -o public03.x

public04.x: public04.o fs-sim.o memory-checking.o
	$(CC) public04.o fs-sim.o memory-checking.o -o public04.x

public05.x: public05.o fs-sim.o memory-checking.o driver.o
	$(CC) public05.o fs-sim.o memory-checking.o driver.o -o public05.x

public06.x: public06.o fs-sim.o
	$(CC) public06.o fs-sim.o -o public06.x

public07.x: public07.o fs-sim.o
	$(CC) public07.o fs-sim.o -o public07.x

public08.x: public08.o fs-sim.o
	$(CC) public08.o fs-sim.o -o public08.x

public09.x: public09.o fs-sim.o driver.o
	$(CC) public09.o fs-sim.o driver.o -o public09.x

public10.x: public10.o fs-sim.o driver.o
	$(CC) public10.o fs-sim.o driver.o -o public10.x

fs-sim.o: fs-sim.c fs-sim.h fs-sim-datastructure.h
	$(CC) $(CFLAGS) -c fs-sim.c

public01.o: public01.c fs-sim.h fs-sim-datastructure.h
	$(CC) $(CFLAGS) -c public01.c

public02.o: public02.c fs-sim.h fs-sim-datastructure.h
	$(CC) $(CFLAGS) -c public02.c

public03.o: public03.c fs-sim.h fs-sim-datastructure.h memory-checking.h
	$(CC) $(CFLAGS) -c public03.c

public04.o: public04.c fs-sim.h fs-sim-datastructure.h memory-checking.h
	$(CC) $(CFLAGS) -c public04.c

public05.o: public05.c fs-sim.h fs-sim-datastructure.h memory-checking.h \
	    driver.h
	$(CC) $(CFLAGS) -c public05.c

public06.o: public06.c fs-sim.h fs-sim-datastructure.h
	$(CC) $(CFLAGS) -c public06.c

public07.o: public07.c fs-sim.h fs-sim-datastructure.h
	$(CC) $(CFLAGS) -c public07.c

public08.o: public08.c fs-sim.h fs-sim-datastructure.h
	$(CC) $(CFLAGS) -c public08.c

public09.o: public09.c fs-sim.h fs-sim-datastructure.h driver.h
	$(CC) $(CFLAGS) -c public09.c

public10.o: public10.c fs-sim.h fs-sim-datastructure.h driver.h
	$(CC) $(CFLAGS) -c public10.c

clean:
	rm -f *.x fs-sim.o public01.o public02.o public03.o public04.o \
	          public05.o public06.o public07.o public08.o public09.o \
		  public10.o
