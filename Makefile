CFLAGS = -Wall -std=c99 -pedantic -g

asgn6:	asgn6.c
	gcc  $(CFLAGS) asgn6.c -o asgn6

test:	asgn6
	asgn6 $(HOME)/cpe357/asgn6 county_demographics.csv four.ops

valgrind: asgn6
	valgrind -s --leak-check=yes ./asgn6 county_demographics.csv three.ops
