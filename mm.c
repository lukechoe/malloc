/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "team",
    /* First member's full name */
    "luke choe",
    /* First member's email address */
    "lukechoe2020@U.northwestern.edu",
    /* Second member's full name (leave blank if none) */
    "tba",
    /* Second member's email address (leave blank if none) */
    "tba"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{	
	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	/*
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }

	*/

	int newsize = ALIGN(size + SIZE_T_SIZE);


	if (mem_heapsize() == 0){
		
		size_t* p = mem_sbrk(newsize);
		if (p == (void*) -1)
			return NULL;
		*p = newsize; 
		*p = 0x3 | *p;
		return (void*) (char*) p + SIZE_T_SIZE;		
	}
	
	void* temp = mem_heap_lo; 
	size_t * T = temp;
	
	//T -= SIZE_T_SIZE;  	
//	printf("%zu malloc\n",*T );	
	while ((*T & 0x2)  != 0x2){
		//size_t actualsize = *T & 0x7FFF; 
		//printf("testing\n\n");
		if ((*T & 0x1) == 0 && (*T >= size)){	
			return (void*) (char*) T+SIZE_T_SIZE;
		}
		 
		T += (*T & 0xffffFFFD)+ SIZE_T_SIZE; 
		 
	}

	if ((*T & 0x2) == 0x2){
		*T = *T & 0xFFFFFFFD; //  *T &  1111...1101 
	}
	//printf("%zu\n", newsize);
	size_t* p = mem_sbrk(newsize);
//	size_t* q = p + newsize;
//	*q = 0xFFFFFFFF;   
	if (p == (void *) -1)
		return NULL;
	*p = newsize;
	
	*p = 0x3 | *p; //put 1's in the 0th and 1st index bits... 0th index for allocated vs not allocated... 1st index bit for "last part of heap?"
	return (void*) (char *) p + SIZE_T_SIZE;	
	 			
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
			 			
	size_t* temp = ptr;
	temp -= SIZE_T_SIZE;  
	*temp = 0xfffffffe & *temp ;  	
	 
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














