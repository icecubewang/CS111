#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>		/* for clock_gettime */
#include <pthread.h>
#include <sched.h> 		/* for sched_yield */

/* Global variables */
int noOfThreads = 1;
int noOfIterations = 1;
int opt_yield = 0;

/* Check return value function */
void check_return_value(int ret, int errnum) {
	if (ret < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errnum));
		exit(1);
	}
}

/* Add function */
void add(long long *pointer, long long value) {
	long long sum = *pointer + value;
	if (opt_yield)
		sched_yield();
	*pointer = sum;
}

/* Aggregated add function */
void *addfunction(void *pointer) {
	long long * counter = (long long *) pointer;
	for (int i = 0; i < noOfIterations; i++) {
		add(counter, 1);
	}
	for (int i = 0; i < noOfIterations; i++) {
		add(counter, -1);
	}
}

int main(int argc, char *argv[]) {
	/* Parse arguments */
	int c;
	struct option long_options[] = {
		{"threads", required_argument, 0, 't'},
		{"iterations", required_argument, 0, 'i'},
		{"yield", no_argument, 0, 'y'},
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
			case 'y':
				opt_yield = 1;
				break;
			default:
				fprintf(stderr, "Usage: ./lab2_add --threads=[number] --iterations=[number]\n");
				exit(1);
		}
	}

	long long counter = 0;

	/* Get starting time */
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	/* Start threads */
	pthread_t threads[noOfThreads];
	for (int i = 0; i < noOfThreads; i++) {
		int ret = pthread_create(&threads[i], NULL, addfunction, (void *) &counter);
		int errnum = errno;
		check_return_value(ret, errnum);
	}
	for (int i = 0; i < noOfThreads; i++) {
		int ret = pthread_join(threads[i], NULL);
		int errnum = errno;
		check_return_value(ret, errnum);
	}

	/* Get ending time */
	clock_gettime(CLOCK_MONOTONIC, &end);

	/* Get total run time */
	long long elapsedTime;
	elapsedTime = 1000000000 * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);

	/* Print CSV */
	int noOfOps = noOfThreads * noOfIterations * 2;
	int timePerOps = elapsedTime / noOfOps;
	char msg[1000];
	printf("%d\n", counter);
	char* begin;
	if (opt_yield) {
		begin = "add-yield";
	}
	else {
		begin = "add-none";
	}
	sprintf(msg, "%s,%d,%d,%d,%d,%d,%d\n", begin, noOfThreads, noOfIterations, noOfOps, elapsedTime, timePerOps, counter);
	int ret;
	ret = write(STDOUT_FILENO, msg, strlen(msg));
	int errnum = errno;
	check_return_value(ret, errnum);

	exit(0);
}