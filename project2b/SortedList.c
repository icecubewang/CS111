//NAME: Feilan Wang
//EMAIL: wangfeilan@hotmail.com
//ID: 104796844

#ifndef SORTEDLIST_H
#define SORTEDLIST_H
#include "SortedList.h"
#endif
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
	//Edge case: empty list
	if (list->next == NULL) {
		if (opt_yield & INSERT_YIELD) sched_yield();
		list->next = element;
		element->prev = list;
		return;
	}
	SortedListElement_t *p = list;
	//Insert at the middle
	while (p != NULL && p->next != NULL) {
		p = p->next;
		if (*(p->key) >= *(element->key)) {
			SortedListElement_t *pprev = p->prev;
			if (opt_yield & INSERT_YIELD) sched_yield();
			pprev->next = element;
			element->prev = pprev;
			p->prev = element;
			element->next = p;
			return;
		}
	}
	//Insert at the end
	if (opt_yield & INSERT_YIELD) sched_yield();
	p->next = element;
	element->prev = p;
	return;
}

int SortedList_delete(SortedListElement_t *element) {
	SortedListElement_t *p = element;
	SortedListElement_t *pprev = element->prev;
	SortedListElement_t *pnext = element->next;
	if (pprev == NULL) return 1; //corrupted
	else {
		if (opt_yield & DELETE_YIELD) sched_yield();
		pprev->next = pnext;
		p->prev = NULL;
	}
	if (pnext == NULL) return 0; //delete the last element
	else {
		pnext->prev = pprev;
		p->next = NULL;
	}
	return 0; //delete the front/middle element
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
	SortedListElement_t *p = list;
	while (p != NULL && p->next != NULL) {
		p = p->next;
		if (opt_yield & LOOKUP_YIELD) sched_yield();
		if (*(p->key) == *(key)) {
			return p;
		}
	}
	return NULL;
}

int SortedList_length(SortedList_t *list) {
	int len = 0;
	SortedListElement_t *p = list;
	if (p == NULL) return -1;
	while (p->next != NULL) {
		p = p->next;
		len += 1;
	}
	return len;
}