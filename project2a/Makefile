#NAME: Feilan Wang
#EMAIL: wangfeilan@hotmail.com
#ID: 

CC = gcc
CFLAGS = -O1 -Wall -Wextra -pthread -g

all: lab2_add lab2_list

lab2_add: lab2_add.c
	${CC} ${CFLAGS} -pthread -o $@ $^

lab2_list: lab2_list.c SortedList.c SortedList.h
	${CC} ${CFLAGS} -pthread -o $@ $^

tests:
	chmod +x gnuplotGenerate.sh
	-./gnuplotGenerate.sh
	
graphs:
	gnuplot lab2_add.gp
	gnuplot lab2_list.gp

dist: all tests graphs
	tar -cvzf lab2a-104796844.tar.gz *.c *.h Makefile *.png README gnuplotGenerate.sh *.gp *.csv

clean:
	-rm -f lab2a-xxxxxxxxx.tar.gz lab2_add lab2_list *.o 
