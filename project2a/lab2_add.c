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
char begin[20];
int opt_yield = 0;
int opt_sync = 0;
int opt_m = 0;
int opt_s = 0;
int opt_c = 0;
pthread_mutex_t lock_mutex;
int lock_testAndSet = 0;

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
	if (opt_yield) sched_yield();
	*pointer = sum;
}

/* Aggregated add function */
void *addfunction(void *pointer) {
	long long * counter = (long long *) pointer;
	if (!opt_c) {
		if (opt_m) pthread_mutex_lock(&lock_mutex);
		if (opt_s) while (__sync_lock_test_and_set(&lock_testAndSet, 1));
		for (int i = 0; i < noOfIterations; i++) {
			add(counter, 1);
		}
		if (opt_s) __sync_lock_release(&lock_testAndSet);
		if (opt_m) pthread_mutex_unlock(&lock_mutex);

		if (opt_m) pthread_mutex_lock(&lock_mutex);
		if (opt_s) while (__sync_lock_test_and_set(&lock_testAndSet, 1));
		for (int i = 0; i < noOfIterations; i++) {
			add(counter, -1);
		}
		if (opt_s) __sync_lock_release(&lock_testAndSet);
		if (opt_m) pthread_mutex_unlock(&lock_mutex);
	}
	else {
		for (int i = 0; i < noOfIterations; i++) {
			long long old, new;
			do {
				old = *(long long *) pointer;
				new = old + 1;
				if (opt_yield) sched_yield();
			}
			while (__sync_val_compare_and_swap((long long *)pointer, old, new) != old);
		}
		for (int i = 0; i < noOfIterations; i++) {
			long long old, new;
			do {
				old = *(long long *) pointer; 
				new = old - 1;
				if (opt_yield) sched_yield();
			}
			while (__sync_val_compare_and_swap((long long *)pointer, old, new) != old);
		}
	}
}

int main(int argc, char *argv[]) {
	/* Parse arguments */
	strcpy(begin, "add-");
	int c;
	struct option long_options[] = {
		{"threads", required_argument, 0, 't'},
		{"iterations", required_argument, 0, 'i'},
		{"yield", no_argument, 0, 'y'},
		{"sync", required_argument, 0, 's'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	while ((c = getopt_long(argc, argv, ":t:iy:s", long_options, &option_index)) != -1) {
		switch(c) {
			case 't':
				noOfThreads = atoi(optarg);
				break;
			case 'i':
				noOfIterations = atoi(optarg);
				break;
			case 'y':
				opt_yield = 1;
				strcat(begin, "yield-");
				break;
			case 's':
				opt_sync = 1;
				if (*optarg == *("m")) {
					opt_m = 1;
					strcat(begin, "m");
				}
				else if (*optarg == *("s")) {
					opt_s = 1;
					strcat(begin, "s");
				}
				else if (*optarg == *("c")) {
					opt_c = 1;
					strcat(begin, "c");
				}
				else {
					fprintf(stderr, "Usage: ./lab2_add --threads=[INT] --iterations=[INT] --yield --sync=[msc]\n");
					exit(1);
				}
				break;
			default:
				fprintf(stderr, "Usage: ./lab2_add --threads=[INT] --iterations=[INT] --yield --sync=[msc]\n");
				exit(1);
		}
	}
	if (!opt_sync) strcat(begin, "none");

	/* Initialization */
	long long counter = 0;
	int ret;
	if (opt_m) {
		ret = pthread_mutex_init(&lock_mutex, NULL);
		int errnum = errno;
		check_return_value(ret, errnum);
	}

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
	sprintf(msg, "%s,%d,%d,%d,%d,%d,%d\n", begin, noOfThreads, noOfIterations, noOfOps, elapsedTime, timePerOps, counter);
	ret = write(STDOUT_FILENO, msg, strlen(msg));
	int errnum = errno;
	check_return_value(ret, errnum);

	pthread_mutex_destroy(&lock_mutex);
	exit(0);
}