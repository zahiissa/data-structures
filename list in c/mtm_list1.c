/*
 * list_mtm1.c
 *
 *  Created on: Dec 2, 2014
 *      Author: user
 */
#include <stdlib.h>
#include "list_mtm1.h"
#define NULL_CHECK_DESTROY(var,lsit,rtn_val)  if(var == NULL){ listDestroy(list); return rtn_val;}
#define EMPTY_CHECK(list,rtn_val) if(list->size == 0){return rtn_val;}
#define NULL_CHECK(arg,rtn_val) if(arg == NULL) {return rtn_val;}


typedef struct Node_t *Node;
struct Node_t {
   ListElement element;
	Node next;
};

struct List_t {
	int size;
	Node head;
	Node tail;
	Node iterator;
	CopyListElement copyElement;
	FreeListElement freeElement;
};
static bool validIterator(List list){
	Node orgIterator=list->iterator;
	if(list->iterator==list->head){
		return true;
	}
	Node itr= list->head->next;
	while (itr != list->tail) {
		if(itr==orgIterator){
			return true;
		}
		itr=itr->next;
	}
	if(itr==orgIterator){
		return true;
	}
	return false;
}

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
	// NULL CHECK FOR PASSED PARAMETER

	NULL_CHECK(copyElement, NULL);
	NULL_CHECK(freeElement, NULL);


	List list =malloc(sizeof(*list));
	NULL_CHECK(list, NULL);

	Node head =(Node)malloc(sizeof(*head));
	if (head == NULL) {
		free(list);
		return NULL;
	}
	Node tail =(Node)malloc(sizeof(*tail));
	if (tail == NULL) {
		free(head);
		free(list);
		return NULL;
	}
	list->head = head;
	list->tail = tail;
	list->size = 0;
	list->head->next = list->tail;
	list->tail->next = NULL;
	list->iterator = list->head;
	list->copyElement = copyElement;
	list->freeElement = freeElement;

	// RETURN THE CREATED priorityQueue
	return list;
}

int listGetSize(List list) {
	NULL_CHECK(list, -1);
	return list->size;
}

ListElement listGetFirst(List list) {
	NULL_CHECK(list, NULL);
	EMPTY_CHECK(list, NULL);
	list->iterator = list->head->next;
	return list->iterator->element;
}


ListElement listGetCurrent(List list) {
	NULL_CHECK(list, NULL);
	return list->iterator->element;
}
ListResult listInsertFirst(List list, ListElement element) {
	NULL_CHECK(list, LIST_NULL_ARGUMENT);
	NULL_CHECK(element, LIST_NULL_ARGUMENT);
	Node tmpIterator = list->iterator;
	list->iterator = list->head;
	listInsertAfterCurrent(list, element);
	list->iterator = tmpIterator;
	return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
	NULL_CHECK(list, LIST_NULL_ARGUMENT);
	NULL_CHECK(element, LIST_NULL_ARGUMENT);
	Node tmpIterator = list->iterator;
	list->iterator = list->tail;
	listInsertBeforeCurrent(list, element);
	list->iterator = tmpIterator;
	return LIST_SUCCESS;

}

static Node getPrev(List list, Node node) {

	Node staticIt = list->head;
	for (int i=0 ; i<listGetSize(list) +1 ; i++) {

		if (staticIt->next == node) {
			return staticIt;
		}
		staticIt = staticIt->next;
	}

	return NULL;
}
ListElement listGetNext(List list) {
	NULL_CHECK(list, NULL);
	NULL_CHECK(list->iterator, NULL);
	if( !validIterator(list) || list->iterator->next == list->tail){
		return NULL;
	}
	list->iterator = list->iterator->next;

	return list->iterator->element;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
	NULL_CHECK(list, LIST_NULL_ARGUMENT);
	NULL_CHECK(element, LIST_NULL_ARGUMENT);
		if(!validIterator(list) || list->iterator==list->head){
			return LIST_NULL_ARGUMENT;
		}
	Node elementNode =(Node)malloc(sizeof(*elementNode));
	NULL_CHECK(elementNode, LIST_OUT_OF_MEMORY);
	elementNode->element = element;
	Node prev = getPrev(list, list->iterator);
	prev->next = elementNode;
	elementNode->next = list->iterator;
	list->size++;
	return LIST_SUCCESS;

}

ListResult listInsertAfterCurrent(List list, ListElement element) {
	NULL_CHECK(list, LIST_NULL_ARGUMENT);
	NULL_CHECK(element, LIST_NULL_ARGUMENT);
		if(!validIterator(list) || list->iterator==list->tail){
			return LIST_NULL_ARGUMENT;
		}
	Node elementNode = (Node)malloc(sizeof(*elementNode));
	NULL_CHECK(elementNode, LIST_OUT_OF_MEMORY);
	elementNode->element = element;
	Node tmp = list->iterator->next;
	list->iterator->next = elementNode;
	elementNode->next = tmp;
	list->size++;
	return LIST_SUCCESS;

}
List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
	NULL_CHECK(list, NULL);
	NULL_CHECK(filterElement,NULL);
	List newList = listCreate(list->copyElement, list->freeElement);
	if (!newList) {
		return NULL;
	}
	LIST_FOREACH(ListElement,itr,list) {
		if (filterElement(itr,key)) {
			ListResult result = listInsertLast(newList,itr);
			if (result == LIST_OUT_OF_MEMORY){
				listDestroy(newList);
				return NULL;
				}
			}
		}
	return newList;
}
static bool alwaysTrue() {
	return true;
}

List listCopy(List list) {
	return listFilter(list, alwaysTrue, NULL);
}

ListResult listClear(List list) {
	NULL_CHECK(list, LIST_NULL_ARGUMENT);
	list->iterator = list->head->next;
	while (list->iterator!= list->tail) {
		Node tmp = list->iterator->next;
		list->freeElement(list->iterator->element);
	//	free(list->iterator); //                 hooon el3'l6a
		list->iterator = tmp;
	}
	list->size=0;
	return LIST_SUCCESS;
}

void listDestroy(List list) {
	if(list==NULL){
		return;
	}
	listClear(list);
	free(list->head);
	free(list->tail);
	free(list);
}

ListElement getMaxElement(List list,int length,CompareListElements compareFunction){
	Node tmp=list->iterator;
	list->iterator = list->head->next;
	Node max =list->iterator;

	for (int i =0 ;i < length ; i++) {
		if ( (compareFunction(max->element,list->iterator->element)) < 0 ) {
			max = list->iterator ;
		}
		list->iterator=list->iterator->next;
	}
	list->iterator=tmp;
	return max;
}

ListResult listSort(List list, CompareListElements compareFunction) {
	NULL_CHECK(list, LIST_NULL_ARGUMENT);
	NULL_CHECK(compareFunction, LIST_NULL_ARGUMENT);
	if (list->size <= 1) {
		return LIST_SUCCESS;
	}
	Node orgIterator=list->iterator;
	for (int i =listGetSize(list); i > 0 ; i--) {
    	Node max = getMaxElement (list,i,compareFunction);
    	ListElement element=max->element;
    	if (listInsertLast(list,element)== LIST_OUT_OF_MEMORY){
    		return LIST_OUT_OF_MEMORY;
    	}
    	list->iterator=max;
    	listRemoveCurrent(list);
    	list->iterator=orgIterator;
    }
	return LIST_SUCCESS;

}

ListResult listRemoveCurrent(List list) {
	NULL_CHECK(list, LIST_NULL_ARGUMENT);
	if(!validIterator(list)||list->iterator==list->head||list->iterator==list->tail){
		return LIST_INVALID_CURRENT;
	}
	Node prev = getPrev(list, list->iterator);
	Node after = list->iterator->next;
	list->freeElement(list->iterator->element);
	prev->next = after;
	list->size--;
	return LIST_SUCCESS;
}

