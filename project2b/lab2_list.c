//NAME: Feilan Wang
//EMAIL: wangfeilan@hotmail.com
//ID: 104796844

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
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
int noOfLists = 1;
int noTI;
int opt_yield = 0;
int yield_i = 0;
int yield_d = 0;
int yield_l = 0;
int sync_s = 0;
int sync_m = 0;
char yieldopts[5];
char syncopts[5];
SortedList_t* list;
SortedListElement_t* elements;
int intarr[100];
pthread_mutex_t* lock_mutex;
int* lock_testAndSet;
long long* total_lock_waiting_time;

/* Signal Handler for Segmentation Fault */
void sighandler(int signum) {
	if (signum == 11) {
		fprintf(stderr, "SegFault: Corrupted List. Error message: %s\n", strerror(errno));
		exit(2);
	}
}

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

/* Aggregated list function for spliting list implimentation */
void *listfunctionB(void *pointer) {
	int * index = (int *) pointer;

	//Insert
	for (int i = 0; i < noOfIterations; i++) {

		int location = (*index) * noOfIterations + i;

		//hash function for the key to be inserted
		//hash function should be key % noOfLists
		int whichlist = ((int)*(elements[location].key)) % noOfLists;

		//Insert into the corresponding list
		SortedList_t *thislist = &list[whichlist];
		if (sync_m) {
			struct timespec lock_start, lock_end;
			clock_gettime(CLOCK_MONOTONIC, &lock_start);
			pthread_mutex_lock(&lock_mutex[whichlist]);
			clock_gettime(CLOCK_MONOTONIC, &lock_end);
			total_lock_waiting_time[*index] += 1000000000 * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);
		}
		if (sync_s) {
			struct timespec lock_start, lock_end;
			clock_gettime(CLOCK_MONOTONIC, &lock_start);
			while (__sync_lock_test_and_set(&lock_testAndSet[whichlist], 1));
			clock_gettime(CLOCK_MONOTONIC, &lock_end);
			total_lock_waiting_time[*index] += 1000000000 * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);
		}
	
		SortedList_insert(thislist, &elements[location]);
		if (sync_s) {
			__sync_lock_release(&lock_testAndSet[whichlist]);
		}
		if (sync_m) pthread_mutex_unlock(&lock_mutex[whichlist]);

	}

	//Length
	int length = 0;
	for (int i = 0; i < noOfLists; i++) {

		if (sync_m) {
			struct timespec lock_start, lock_end;
			clock_gettime(CLOCK_MONOTONIC, &lock_start);
			pthread_mutex_lock(&lock_mutex[i]);
			clock_gettime(CLOCK_MONOTONIC, &lock_end);
			total_lock_waiting_time[*index] += 1000000000 * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);
		}
		if (sync_s) {
			struct timespec lock_start, lock_end;
			clock_gettime(CLOCK_MONOTONIC, &lock_start);
			while (__sync_lock_test_and_set(&lock_testAndSet[i], 1));
			clock_gettime(CLOCK_MONOTONIC, &lock_end);
			total_lock_waiting_time[*index] += 1000000000 * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);
		}
	}
	for (int i = 0; i < noOfLists; i++) {
		length += SortedList_length(&list[i]);
	}
	for (int i = 0; i < noOfLists; i++) {
		if (sync_s) {
			__sync_lock_release(&lock_testAndSet[i]);
		}
		if (sync_m) {
			pthread_mutex_unlock(&lock_mutex[i]);
		}
	}
	if (length <= 0) corrupted_msg();

	//Look up and Delete
	for (int i = 0; i < noOfIterations; i++) {

		int location = (*index) * noOfIterations + i;

		//hash function for the key to be inserted
		//hash function should be key % noOfLists
		int whichlist = ((int)*(elements[location].key)) % noOfLists;

		//Look up in the corresponding list
		SortedList_t *thislist = &list[whichlist];

		if (sync_m) {
			struct timespec lock_start, lock_end;
			clock_gettime(CLOCK_MONOTONIC, &lock_start);
			pthread_mutex_lock(&lock_mutex[whichlist]);
			clock_gettime(CLOCK_MONOTONIC, &lock_end);
			total_lock_waiting_time[*index] += 1000000000 * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);
		}
		if (sync_s) {
			struct timespec lock_start, lock_end;
			clock_gettime(CLOCK_MONOTONIC, &lock_start);
			while (__sync_lock_test_and_set(&lock_testAndSet[whichlist], 1));
			clock_gettime(CLOCK_MONOTONIC, &lock_end);
			total_lock_waiting_time[*index] += 1000000000 * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);
		}

		SortedListElement_t* delete = SortedList_lookup(thislist, elements[location].key);
		if (delete == NULL) corrupted_msg();
		else {
			int ret = SortedList_delete(delete);
			if (ret == 1) corrupted_msg();
		}
		if (sync_s) __sync_lock_release(&lock_testAndSet[whichlist]);
		if (sync_m) pthread_mutex_unlock(&lock_mutex[whichlist]);

	}

	return NULL;
}

int main(int argc, char *argv[]) {
	/* Signal handler */
	signal(SIGSEGV, sighandler);

	/* Parse arguments */
	int c;
	struct option long_options[] = {
		{"threads", required_argument, 0, 't'},
		{"iterations", required_argument, 0, 'i'},
		{"yield", required_argument, 0, 'y'},
		{"sync", required_argument, 0, 's'},
		{"lists", required_argument, 0, 'l'},
		{0, 0, 0, 0}
	};
	int option_index = 0;
	int yield_len;
	while ((c = getopt_long(argc, argv, ":t:i:y:s:l", long_options, &option_index)) != -1) {
		switch(c) {
			case 't':
				noOfThreads = atoi(optarg);
				if (noOfThreads <= 0) {
					fprintf(stderr, "Usage: ./lab2_list --threads=[POSINT] --iterations=[POSINT] --yield=[idl] --sync=[ms] --lists=[POSINT]\n");
					exit(1);
				}
				break;
			case 'i':
				noOfIterations = atoi(optarg);
				if (noOfIterations <= 0) {
					fprintf(stderr, "Usage: ./lab2_list --threads=[POSINT] --iterations=[POSINT] --yield=[idl] --sync=[ms] --lists=[POSINT]\n");
					exit(1);
				}
				break;
			case 'y':
				yield_len = strlen(optarg);
				for (int i = 0; i < yield_len; i++) {
					char c = *(optarg+i);
					switch(c) {
						case 'i':
							opt_yield += 1;
							yield_i = 1;
							strcat(yieldopts, "i");
							break;
						case 'd':
							opt_yield += 2;
							yield_d = 1;
							strcat(yieldopts, "d");
							break;
						case 'l':
							opt_yield += 4;
							yield_l = 1;
							strcat(yieldopts, "l");
							break;
						default:
							fprintf(stderr, "Usage: ./lab2_list --threads=[INT] --iterations=[INT] --yield=[idl] --sync=[ms] --lists=[POSINT]\n");
							exit(1);
					}
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
				break;
			case 'l':
				noOfLists = atoi(optarg);
				if (noOfLists <= 0) {
					fprintf(stderr, "Usage: ./lab2_list --threads=[INT] --iterations=[INT] --yield=[idl] --sync=[ms] --lists=[POSINT]\n");
					exit(1);
				}
				break;
			default:
				fprintf(stderr, "Usage: ./lab2_list --threads=[INT] --iterations=[INT] --yield=[idl] --sync=[ms] --lists=[POSINT]\n");
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
	if (sync_m) {
		lock_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * noOfLists);
		int ret;
		for (int i = 0; i < noOfLists; i++) {
			ret = pthread_mutex_init(&lock_mutex[i], NULL);
			int errnum = errno;
			check_return_value(ret, errnum);
		}
	}

	if (sync_s) {
		lock_testAndSet = (int *)calloc(noOfLists, sizeof(int));
	}

	total_lock_waiting_time = (long long *)calloc(noOfThreads, sizeof(long long));


	noTI = noOfThreads * noOfIterations;
	list = (SortedList_t *)malloc(sizeof(SortedList_t) * noOfLists);
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
		int ret = pthread_create(&threads[i], NULL, listfunctionB, (void *) &id[i]);
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
	int length = 0;
	for (int i = 0; i < noOfLists; i++) {
		length += SortedList_length(&list[i]);
	}
	if (length != 0) corrupted_msg();

	/* Get total run time */
	long long elapsedTime;
	elapsedTime = 1000000000 * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);

	/* Get total waiting time from per thread */
	long long totalTime = 0;
	for (int i = 0; i < noOfThreads; i++) {
		totalTime += total_lock_waiting_time[i];
	}
	long long average_waiting_for_lock = (long long) totalTime / (noOfThreads * (noOfIterations * 2 + 1));

	/* Print CSV */
	int noOfOps = noTI * 3;
	long long timePerOps = (long long) elapsedTime / noOfOps;

	char msg[1000];

	sprintf(msg, "list-%s-%s,%d,%d,%d,%d,%lld,%lld,%lld\n", yieldopts, syncopts, noOfThreads, noOfIterations, noOfLists, noOfOps, elapsedTime, timePerOps, average_waiting_for_lock);
	int ret = write(STDOUT_FILENO, msg, strlen(msg));
	int errnum = errno;
	check_return_value(ret, errnum);

	// for (int i = 0; i < noOfLists; i++) {
	// 	pthread_mutex_destroy(&lock_mutex[i]);
	// }

	/* Free element */
	free(elements);
	free(list);
	if (sync_m || sync_s) free(total_lock_waiting_time);
	if (sync_m) free(lock_mutex);
	if (sync_s) free(lock_testAndSet);

	exit(0);
}