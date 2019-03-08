#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "list_mtm1.h"
#include "string.h"

/**
 * These macros are here to help you create tests more easily and keep them
 * clear
 *
 * The basic idea with unit-testing is create a test function for every real
 * function and inside the test function declare some variables and execute the
 * function under test.
 *
 * Use the ASSERT_TEST to verify correctness of values.
 */

/**
 * Evaluates b and continues if b is true.
 * If b is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
#define ASSERT_TEST(b) do { \
        if (!(b)) { \
                printf("\nAssertion failed at %s:%d %s ",__FILE__,__LINE__,#b); \
                return false; \
        } \
} while (0)

/**
 * Macro used for running a test from the main function
 */
#define RUN_TEST(test) do { \
        printf("Running "#test"... "); \
        if (test()) { \
            printf("[OK]\n");\
        } else { \
        	printf("[Failed]\n"); \
        } \
} while(0)

/**
 * These two macros are to help you initialize a set of examples. Look at
 * list_test.h for an example of how they can be used to save a lot of code
 */
#define SET_UP(Typename) Typename examples = setUp()
#define TEAR_DOWN() tearDown(examples)
/*
 * List of tests, one for each function is usually a good thumb rule.
 */

static ListElement stringCopy(ListElement str) {
	assert(str);
	char* result = malloc(strlen((char*) str) + 1);
	return result ? strcpy(result, (char*) str) : NULL;
}

static void stringFree(ListElement str) {
	free((char*) str);
}
static int stringCompare(ListElement str1,ListElement str2){
	assert(str1);
	assert(str2);
	return strcmp(str1,str2);
}
static List getMyList(){
	List list=listCreate(stringCopy,stringFree);
	listInsertFirst( list,"D");
	listInsertFirst( list,"C");
	listInsertFirst( list,"B");
	listInsertFirst( list,"A");
	listInsertLast( list,"Emelaaaa");
	return list;

}
static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(stringCopy,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,stringFree) == NULL);
	List lst=listCreate(stringCopy,stringFree);
	if(!lst){
		return true;
	}
	listDestroy(lst);
	return true;
}
static bool isLongerThan(ListElement string, ListFilterKey key) {
	  return strlen(string) > *(int*)key;
 }
static bool testListCopy() {
	 List list =getMyList();
	 if(list==NULL){
	 		return true;
	 	}
	 ASSERT_TEST(list);
	 ASSERT_TEST(listCopy(NULL)==NULL);
     List list1 =listCopy(list);
     ASSERT_TEST(list1);
     ASSERT_TEST(listGetSize(list)==listGetSize(list1));
     listDestroy(list);
     listDestroy(list1);
	 return true;
}

static bool testListGetSize() {
	ASSERT_TEST(listGetSize(NULL) == -1);
	return true;
}
static bool testListGetFirst() {
	ASSERT_TEST(listGetFirst(NULL)==NULL);
	List list=getMyList();
	if(list==NULL){
			return true;
		}
	ASSERT_TEST(!stringCompare(listGetFirst(list),"A"));
	listDestroy(list);
	return true;
}

static bool testListGetNext() {
	List list=getMyList();
	if(list==NULL){
			return true;
		}
	ASSERT_TEST(!stringCompare(listGetFirst(list),"A"));
	ASSERT_TEST(!stringCompare(listGetNext(list),"B"));
	ASSERT_TEST(!stringCompare(listGetNext(list),"C"));
	ASSERT_TEST(!stringCompare(listGetNext(list),"D"));
	ASSERT_TEST(!stringCompare(listGetNext(list),"Emelaaaa"));
	ASSERT_TEST(listGetNext(list)==NULL);
	listDestroy(list);

	return true;
}

static bool testListInsertFirst() {
	List list=getMyList();
	if(list==NULL){
		return true;
	}
	ASSERT_TEST(!stringCompare(listGetFirst(list),"A"));
	if(listInsertFirst(list,"Safa")==LIST_SUCCESS){
	ASSERT_TEST(stringCompare(listGetFirst(list),"A"));
	ASSERT_TEST(!stringCompare(listGetFirst(list),"Safa"));
	listDestroy(list);
	}
	return true;
}

static bool testListInsertLast() {
	List list=getMyList();
	ASSERT_TEST(!stringCompare(listGetFirst(list),"A"));
	ASSERT_TEST(listInsertLast(list,"Safa")==LIST_SUCCESS);
	ASSERT_TEST(!stringCompare(listGetNext(list),"B"));
	ASSERT_TEST(!stringCompare(listGetNext(list),"C"));
	ASSERT_TEST(!stringCompare(listGetNext(list),"D"));
	ASSERT_TEST(stringCompare(listGetNext(list),"E")!=0);
	ASSERT_TEST(!stringCompare(listGetNext(list),"Safa"));
	ASSERT_TEST(listGetNext(list)==NULL);
	listDestroy(list);
	return true;
}

static bool testListInsertBeforeCurrent() {
	List list=getMyList();
	ASSERT_TEST(!stringCompare(listGetFirst(list),"A"));
	if(listInsertBeforeCurrent(list,"Shehadi")==LIST_SUCCESS){
		ASSERT_TEST(stringCompare(listGetFirst(list),"A")!=0);
		ASSERT_TEST(!stringCompare(listGetFirst(list),"Shehadi"));
		ASSERT_TEST(listGetNext(list));
		if(listInsertBeforeCurrent(list,"Safa")==LIST_SUCCESS){
			ASSERT_TEST(!stringCompare(listGetFirst(list),"Shehadi"));
			ASSERT_TEST(!strcmp(listGetCurrent(list),"Shehadi"));
			ASSERT_TEST(listGetNext(list));
			ASSERT_TEST(!strcmp(listGetCurrent(list),"Safa"));
			ASSERT_TEST(listGetNext(list));
			ASSERT_TEST(!strcmp(listGetCurrent(list),"A"));
		}
	}
	listDestroy(list);
	return true;
}

static bool testListInsertAfterCurrent() {
	List list=getMyList();
	ASSERT_TEST(!stringCompare(listGetFirst(list),"A"));
	if(listInsertAfterCurrent(list,"matam")==LIST_SUCCESS){
		ASSERT_TEST(!stringCompare(listGetFirst(list),"A"));
		ASSERT_TEST(listGetNext(list));
		ASSERT_TEST(!strcmp(listGetCurrent(list),"matam"));
		ASSERT_TEST(listGetNext(list));
		ASSERT_TEST(listGetNext(list));
		ASSERT_TEST(listGetNext(list));
			if(listInsertAfterCurrent(list,"is nice")==LIST_SUCCESS){
				ASSERT_TEST(listGetNext(list));
				ASSERT_TEST(!strcmp(listGetCurrent(list),"is nice"));
				ASSERT_TEST(listGetNext(list));
				ASSERT_TEST(listGetNext(list)==NULL);
			}
	}
	listDestroy(list);
	return true;
}

static bool testListRemoveCurrent() {
	List list=getMyList();
	ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
	ASSERT_TEST(strcmp(listGetFirst(list),"A")==0);
	ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
	ASSERT_TEST(listGetSize(list)==4);
	ASSERT_TEST(strcmp(listGetFirst(list),"A")!=0);


	return true;
}

static bool testListSort() {
	List list = listCreate(stringCopy,stringFree);
	if(!list){
		return true;
	}
	ASSERT_TEST(listSort(NULL,NULL)==LIST_NULL_ARGUMENT);
	ASSERT_TEST(listSort(list,stringCompare)==LIST_SUCCESS);
	ASSERT_TEST(listSort(NULL,stringCompare)==LIST_NULL_ARGUMENT);
	listDestroy(list);

	return true;
}

static bool testListClear() {
	List list=getMyList();
	if(!list){
		return true;
	}
	ASSERT_TEST(list);
	listClear(list);
	ASSERT_TEST(listGetSize(list)==0);
	ASSERT_TEST(list);
	listDestroy(list);
	ASSERT_TEST(listClear(NULL)==LIST_NULL_ARGUMENT);
	return true;
}

static bool testListDestroy() {
	listDestroy(NULL);
	List list1 =getMyList();
	if(!list1){
		return true;
	}
	listDestroy(list1);
	return true;
}
static bool testListFilter(){
	 List list1 = listCreate(stringCopy,stringFree);
	 if(!list1){
		 return true;
	 }
	 int key=5;
	 listInsertLast( list1,"Emelaaaa");
	 listInsertLast(list1,"Safa");
	 ASSERT_TEST(listGetSize(list1)==2);
	 List newList=listFilter(list1,isLongerThan,&key);
	 if(!newList){
		 listDestroy(list1);
		 return true;
	 }
	 ASSERT_TEST(listGetSize(newList)==1);
	 listDestroy(newList);
	 listDestroy(list1);
	 return true;

}

int main() {
	RUN_TEST(testListFilter);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListCreate);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	return 0;
}
