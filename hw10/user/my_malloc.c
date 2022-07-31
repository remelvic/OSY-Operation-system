#include <stdio.h>
#include <stddef.h>


static void *nbrk(void *address)
{
    void *current_brk = sbrk(0);
    if (address != NULL) {
        int ret = brk(address);
        if (ret == -1)
            return NULL;
    }
    return current_brk;
}

struct node* head;
struct node* tail;

int first_alloc = 1;

struct node {
    unsigned long size;
    int free; // free = 0 memory is free -- free = 1 memory is in use
    struct node* next;
    struct node *prev;
};


void *my_malloc(unsigned long size){
    if (size == 0){
        return (void*) 0;
    }

    void *break_current  = nbrk(0);
    void *ptr;
    // if first alloc block 
    if (first_alloc){ 
        first_alloc = 0;
        ptr = (void*)nbrk(size+(unsigned long)break_current+sizeof(struct node));
        if (!ptr){
            printf("Error: error in first nbrk in malloc !\n");
            return (void*) 0;
        }
        //work with my allocated block
        struct node* new_node;
        new_node = ptr;
        new_node->size = size;
        new_node->free = 1;
        new_node->next = NULL;
        new_node->prev = NULL;
        //head and tail point to allocate memory
        head = ptr;
        tail = ptr;
        return ptr+sizeof(struct node);
    
    }else{
        struct node* curr = head;
        
        ptr = (void*)nbrk(size+(unsigned long)break_current+sizeof(struct node));
        if(!ptr){
            printf("Error: error in other nbrk in malloc !\n");
            return (void*) 0;
        }
        struct node* new_node;
        new_node = ptr;
        new_node->size = size;
        new_node->free = 1;
        new_node->next = NULL;
        new_node->prev = NULL;

        tail->next = new_node;
        new_node->prev = tail;
        tail = ptr;
        return ptr+sizeof(struct node);
    }
    
}

int my_free(void *address){

}