#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

/* GLOBAL VARIABLES */
int noOfThreads = 1;
int noOfIterations = 1;
long long counter = 0;
int startingTime;

int main(int argc, char *argv[]) {
	/* Parse arguments */
	int c;
	struct option long_options[] = {
		{"threads", required_argument, 0, 't'},
		{"iteration", required_argument, 0, 'i'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	while ((c = getopt_long(argc, argv, ":t:i", long_options, &option_index)) != -1) {
		switch(c) {
			case 't':
				noOfThreads = atoi(optarg);
				break;
			case 'i':
				noOfIterations = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: ./lab2_add --threads=[number] --iterations=[number]\n");
				break;
		}
	}
}