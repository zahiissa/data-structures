/*
 * mtm_set.c
 *
 *  Created on: Aug 17, 2015
 *      Author: Rush
 */

#include "mtm_set.h"
#include "macros.h"
#define INITIAL_SITUATION(set) \
	set->first->next = NULL; \
	set->last = NULL; \
	set->elementsNum = 0; \
	set->current = NULL;

typedef struct node_t {
	SetElement element;
	struct node_t* next;
}*Node;

struct Set_t {
	Node first;
	Node last;
	Node current;
	int elementsNum;
	copySetElements copyElement;
	freeSetElements freeElement;
	compareSetElements compareFunction;
};

Set setCreate(copySetElements copyElement, freeSetElements freeElement,
		compareSetElements compareElements) {
	IF_NULL_RETURN_NULL(copyElement);
	IF_NULL_RETURN_NULL(freeElement);
	IF_NULL_RETURN_NULL(compareElements);
	ALLOCATE_ELEMENT(Set, newSet, 1);
	IF_NULL_RETURN_NULL(newSet);
	ALLOCATE_ELEMENT(Node, imaginaryNode, 1);
	IF_NULL_DO_AND_RETURN_NULL(imaginaryNode, free(newSet)
	;);
	newSet->first = imaginaryNode;
	INITIAL_SITUATION(newSet);
	newSet->copyElement = copyElement;
	newSet->freeElement = freeElement;
	newSet->compareFunction = compareElements;
	return newSet;
}

Set setCopy(Set set) {
	IF_NULL_RETURN_NULL(set);
	Set newSet = setCreate((copySetElements) set->copyElement,
			(freeSetElements) set->freeElement,
			(compareSetElements) set->compareFunction);
	IF_NULL_RETURN_NULL(newSet);
	SET_FOREACH(SetElement,element,set){
		if(setAdd(newSet,element) != SET_SUCCESS){
			setDestroy(newSet);
			return NULL;
		}
	}
	return newSet;
}

SetResult setAdd(Set set, SetElement element) {
	IF_NULL_RETURN_RESULT(set, SET_NULL_ARGUMENT);
	IF_NULL_RETURN_RESULT(element, SET_NULL_ARGUMENT);
	if (setFind(set, element) != NULL) {
		return SET_ITEM_ALREADY_EXISTS;
	}
	ALLOCATE_ELEMENT(Node, newNode, 1);
	IF_NULL_RETURN_RESULT(newNode, SET_OUT_OF_MEMORY);
	newNode->next = NULL;
	newNode->element = set->copyElement(element);
	if (newNode->element == NULL) {
		free(newNode);
		return SET_OUT_OF_MEMORY;
	}
	if (set->last) { //means the set is not empty
		set->last->next = newNode;
		set->last = newNode;
	} else {
		set->last = newNode;
		set->first->next = newNode;
	}
	set->elementsNum++;
	set->current = NULL;
	return SET_SUCCESS;

}

int setGetSize(Set set) {
	if (set == NULL) {
		return -1;
	}
	return set->elementsNum;
}
SetElement setFind(Set set, SetElement element) {
	IF_NULL_RETURN_NULL(set);
	IF_NULL_RETURN_NULL(element);
	Node tmpNode = set->first;
	while (tmpNode->next != NULL) {
		if (set->compareFunction(tmpNode->next->element, element)) {
			set->current = tmpNode->next;
			return set->current;
		}
		tmpNode = tmpNode->next;
	}
	return NULL;
}


bool setIsIn(Set set, SetElement element){
	if(set == NULL || element == NULL){
		return false;
	}
	if(setFind(set,element) != NULL){
		return true;
	}
	return false;
}

SetResult setRemove(Set set, SetElement element) {
	IF_NULL_RETURN_RESULT(set, SET_NULL_ARGUMENT);
	IF_NULL_RETURN_RESULT(element, SET_NULL_ARGUMENT);
	SetElement tmpElement = setFind(set, element);
	IF_NULL_RETURN_RESULT(tmpElement, SET_ITEM_DOES_NOT_EXIST);

	Node tmp = set->first;
	while (tmp->next) {
		if (set->compareFunction(tmp->next->element, element)) {
			if (tmp->next->next == NULL) { //if its the last element
				set->last = tmp;
				set->freeElement(tmp->next->element);
				free(tmp->next);
				tmp->next = NULL;

			} else {

				Node tmp2 = tmp->next->next;
				set->freeElement(tmp->next->element);
				free(tmp->next);
				tmp->next = tmp2;
			}

			set->elementsNum--;
			break;
		}
		tmp = tmp->next;
	}
	set->current = NULL;
	return SET_SUCCESS;
}

SetResult setClear(Set set) {
	IF_NULL_RETURN_RESULT(set, SET_NULL_ARGUMENT);
	SET_FOREACH(SetElement,element,set)
	{
		setRemove(set, element);
		element = setGetFirst(set);
	}
	INITIAL_SITUATION(set);
	return SET_SUCCESS;
}

void setDestroy(Set set) {
	if (set != NULL) {
		setClear(set);
		free(set->first);
		free(set);
	}
}

SetElement setGetFirst(Set set) {
	IF_NULL_RETURN_NULL(set);
	if (set->elementsNum == 0) {
		return NULL;
	}
	set->current = set->first->next;
	return set->first->next->element;
}

SetElement setGetNext(Set set) {
	IF_NULL_RETURN_NULL(set);
	IF_NULL_RETURN_NULL(set->current);
	IF_NULL_DO_AND_RETURN_NULL(set->current->next, set->current = NULL;);
	set->current = set->current->next;
	return set->current->element;
}
