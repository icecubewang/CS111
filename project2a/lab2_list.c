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
#ifndef SORTEDLIST_H
#define SORTEDLIST_H
#include "SortedList.h"
#endif
/* Global Variables */
int noOfThreads = 1;
int noOfIterations = 1;
int noTI;
int opt_yield = 0;
int yield_i = 0;
int yield_d = 0;
int yield_l = 0;
int sync_s = 0;
int sync_m = 0;
char yieldopts[5];
char syncopts[5];
SortedList_t list;
SortedListElement_t* elements;
int intarr[100];

/* Corrupted List error message */
void corrupted_msg() {
	fprintf(stderr, "Error: corrupted list.\n");
	exit(2);
}

/* Check return value function */
void check_return_value(int ret, int errnum) {
	if (ret < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errnum));
		exit(1);
	}
}

/* Aggregated list function */
void *listfunction(void *pointer) {
	int * index = (int *) pointer;
	//Insert #ofIterations of elements into the list
	for (int i = 0; i < noOfIterations; i++) {
		int location = (*index) * noOfIterations + i;
		//printf("%d: %d\n", pthread_self(), location);
		SortedList_insert(&list, &elements[location]);
	}
	//Get the list length
	int length = SortedList_length(&list);
	if (length <= 0) {
		corrupted_msg();
	}
	//Looks up and deletes each of the keys it had previously inserted
	for (int i = 0; i < noOfIterations; i++) {
		int location = (*index) * noOfIterations + i;
		SortedListElement_t* delete = SortedList_lookup(&list, elements[location].key);
		if (delete == NULL) corrupted_msg();
		else {
			int ret = SortedList_delete(delete);
			if (ret == 1) corrupted_msg();
		}
	}
}

int main(int argc, char *argv[]) {
	/* Parse arguments */
	int c;
	struct option long_options[] = {
		{"threads", required_argument, 0, 't'},
		{"iterations", required_argument, 0, 'i'},
		{"yield", required_argument, 0, 'y'},
		{"sync", required_argument, 0, 's'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	while ((c = getopt_long(argc, argv, ":t:i:y:s", long_options, &option_index)) != -1) {
		switch(c) {
			case 't':
				noOfThreads = atoi(optarg);
				break;
			case 'i':
				noOfIterations = atoi(optarg);
				break;
			case 'y':
				opt_yield = 1;
				if (*optarg == *("i")) {
					yield_i = 1;
					strcpy(yieldopts, "i");
				}
				else if (*optarg == *("d")) {
					yield_d = 1;
					strcpy(yieldopts, "d");
				}
				else if (*optarg == *("l")) {
					yield_l = 1;
					strcpy(yieldopts, "l");
				}
				else if (*optarg == *("id")) {
					yield_i = 1;
					yield_d = 1;
					strcpy(yieldopts, "id");
				}
				else if (*optarg == *("il")) {
					yield_i = 1;
					yield_l = 1;
					strcpy(yieldopts, "il");
				}
				else if (*optarg == *("dl")) {
					yield_d = 1;
					yield_l = 1;
					strcpy(yieldopts, "dl");
				}
				else if (*optarg == *("idl")) {
					yield_i = 1;
					yield_d = 1;
					yield_l = 1;
					strcpy(yieldopts, "idl");
				}
				else {
					fprintf(stderr, "Usage: ./lab2_list --threads=[INT] --iterations=[INT] --yield=[idl]\n");
					exit(1);
				}
				break;
			case 's':
				if (*optarg == *("s")) {
					sync_s = 1;
					strcpy(syncopts, "s");
				}
				else if (*optarg == *("m")) {
					sync_m = 1;
					strcpy(syncopts, "m");
				}
			default:
				fprintf(stderr, "Usage: ./lab2_list --threads=[INT] --iterations=[INT] --yield=[idl]\n");
				exit(1);
		}
	}
	if (!yield_i && !yield_d && !yield_l) {
		strcpy(yieldopts, "none");
	}
	if (!sync_s && !sync_m) {
		strcpy(syncopts, "none");
	}

	/* Initialization */
	noTI = noOfThreads * noOfIterations;
	elements = (SortedListElement_t *)malloc(sizeof(SortedListElement_t) * noTI);
	if (elements == 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		exit(1);
	}
	for (int i = 0; i < 100; i++) {
		intarr[i] = i;
	}
	for (int i = 0; i < noTI; i++) {
		int pick = rand() % 100;
		elements[i].key = (char *) &intarr[pick];
	}
	int id[noOfThreads];
	for (int i = 0; i < noOfThreads; i++) {
		id[i] = i;
	}

	/* Get starting time */
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	/* Start threads */
	pthread_t threads[noOfThreads];
	for (int i = 0; i < noOfThreads; i++) {
		int ret = pthread_create(&threads[i], NULL, listfunction, (void *) &id[i]);
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

	/* Check the length of the list and confirm that it is zero */
	if (SortedList_length(&list) != 0) corrupted_msg();

	/* Free element */
	free(elements);
	/* Get total run time */
	long long elapsedTime;
	elapsedTime = 1000000000 * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);

	/* Print CSV */
	int noOfOps = noTI * 3;
	int timePerOps = elapsedTime / noOfOps;

	char msg[1000];

	sprintf(msg, "list-%s-%s,%d,%d,%d,%d,%d,%d\n", yieldopts, syncopts, noOfThreads, noOfIterations, 1,noOfOps, elapsedTime, timePerOps);
	int ret = write(STDOUT_FILENO, msg, strlen(msg));
	int errnum = errno;
	check_return_value(ret, errnum);

	exit(0);
}