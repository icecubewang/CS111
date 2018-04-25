#NAME: Feilan Wang
#EMAIL: wangfeilan@hotmail.com
#ID: XXXXXXXXX

CC = gcc
CFLAGS = -Wall -Wextra -lz -g -std=c99

all: lab1b-client lab1b-server

lab1b-client: lab1b-client.c
	${CC} ${CFLAGS} -o $@ $^

lab1b-server: lab1b-server.c
	${CC} ${CFLAGS} -o $@ $^

dist:
	tar -cvzf lab1b-XXXXXXXXX.tar.gz lab1b-client.c lab1b-server.c Makefile README

clean:
	-rm -f lab1b-XXXXXXXXX.tar.gz lab1b-client lab1b-server logfile
