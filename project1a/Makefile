#NAME: Feilan Wang
#EMAIL: wangfeilan@hotmail.com
#ID: 

CC = gcc
CFLAGS = -D_POSIX_C_SOURCE -Wall -Wextra -g -std=c99 -O3

all: lab1a

lab1a: lab1a.c
	${CC} ${CFLAGS} -o $@ $^

dist:
	tar -cvzf lab1a-104796844.tar.gz lab1a.c Makefile README

clean:
	-rm -f lab1a-104796844.tar.gz lab1a
