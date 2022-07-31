#include "mem_alloc.h"
#include <stdio.h>
#include <stddef.h>
 
 
/*
 * Template for 11malloc. If you want to implement it in C++, rename
 * this file to mem_alloc.cc.
 */
  
static inline void *nbrk(void *address);
  
//#ifdef NOVA
  
/**********************************/
/* nbrk() implementation for NOVA */
/**********************************/
  
static inline unsigned syscall2(unsigned w0, unsigned w1)
{
    asm volatile("   mov %%esp, %%ecx    ;"
                 "   mov $1f, %%edx      ;"
                 "   sysenter            ;"
                 "1:                     ;"
                 : "+a"(w0)
                 : "S"(w1)
                 : "ecx", "edx", "memory");
    return w0;
}
  
static void *nbrk(void *address)
{
    return (void *)syscall2(3, (unsigned)address);
}

  
typedef struct block{
    unsigned long size; // size block
    unsigned long free; // free or not  true if block is free false if block is not free
    struct block *next; // next element
}block_t;
  
block_t *head = NULL;
block_t *tail = NULL;
void *break_current = NULL;
void *break_min = NULL;

block_t *new_free_memory_chunck(unsigned long size){
    block_t *curr = head;
    block_t *new_block = NULL;
    int my_size = 0;

    while(curr){
        if(curr->free && curr->size >= size){
            return curr;
        }else if(curr->free && curr->size < size){
            if (new_block == NULL){
                new_block = curr;
                my_size = curr->size;    
            }else{
                my_size += curr->size;
            }
            if(my_size >= size){
                new_block->free = 1;
                new_block->size = my_size;
                new_block->next = curr->next;
                return new_block;
            }
        }else if(curr->free == 0){
            new_block = NULL;
        }
        curr = curr->next;
    }
    return NULL;
}
 
void *my_malloc(unsigned long size)
{
    if (!size)
        return (void*) 0;

    break_current = nbrk(0);
    block_t *curr = new_free_memory_chunck(size);
     
    if(curr == NULL){
         
        void* block = nbrk((void*)((unsigned long) break_current + size + sizeof(block_t)));
        if (block == NULL){
            return block;
        }else{
  
            curr = (block_t*) block;
            curr->free = 0;
            curr->size = size;
            curr->next = NULL;
            if(head == NULL){
                head = curr;
            }
            if(tail != NULL){
                tail->next = curr;
            }
            tail = curr;
            return (void*) (curr+1);
        }
    }
    if(curr){
        curr->free = 0;     
        return (void*) (curr+1);
    }

    return (void*)0;
}
  
int my_free(void *address)
{   
    break_current = nbrk(0);
    block_t *entry = (block_t*) address - 1;
 
    if (break_current >= address){
        if(entry->free == 0){
            entry->free = 1;
            return 0;
        }
    }
    return -1;
}