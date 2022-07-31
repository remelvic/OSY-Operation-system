#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <string.h>
 
 
int x;
char* text; // text from input
int count = 0;
bool negativ = false; 
 
bool prod_terminate = false;
bool end = false;
 
typedef struct entry{
    char* value;
    int num;
    struct entry *next;
}entry_t;
  
typedef struct {
    entry_t *head;
    entry_t *tail;
    int count; // count of element
}LL_t;
  
//LL_t list = { NULL, NULL, 0 };
//LL_t *lst = &list;
LL_t* lst;
 
  
sem_t sem;
pthread_mutex_t mutex_LL_Lock;
pthread_mutex_t mutex_print_Lock;
 
LL_t *listInit(){
    LL_t *list = (LL_t *)malloc(sizeof(LL_t));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}
 
void PushEnd(char* val, int x, LL_t *list){
    pthread_mutex_lock(&mutex_LL_Lock);
    entry_t *new_entry = malloc(sizeof(entry_t));
    new_entry->value = val;
    new_entry->num = x;
    if(list->count == 0){
        list->head = list->tail = new_entry;
    }else{
        list->tail->next = new_entry;
        list->tail = new_entry;
    }
    list->count +=1;
    pthread_mutex_unlock(&mutex_LL_Lock);
}
  
bool isEmpty(LL_t *list){
    pthread_mutex_lock(&mutex_LL_Lock);
    bool ret = list->count == 0;
    pthread_mutex_unlock(&mutex_LL_Lock);
    return ret;
}
  
void removeALL(LL_t *list){
    if(isEmpty(list)){
        return;
    }else{
        pthread_mutex_lock(&mutex_LL_Lock);
        entry_t *tmp = list->head;
        while(tmp != NULL){
            entry_t *entry = tmp->next;
            free(tmp);
            tmp = entry;
        }
        pthread_mutex_unlock(&mutex_LL_Lock);
    }
}
  
entry_t *pop(LL_t *list){
    pthread_mutex_lock(&mutex_LL_Lock);
    entry_t *prev_head = list->head;
    if(list != NULL){
        list->head = prev_head->next;
        list->count--;
    }
    pthread_mutex_unlock(&mutex_LL_Lock);
    return prev_head;
}
  
void* producer(void *arg){
    int ret;
    while((ret = scanf("%d %ms", &x, &text)) == 2){
        PushEnd(text,x,lst);
        count++;
        sem_post(&sem);                                 ///
    }
    pthread_mutex_lock(&mutex_LL_Lock);
    end = true;
    //sem_post(&sem);
    pthread_mutex_unlock(&mutex_LL_Lock);
      
    if(ret != EOF){
        prod_terminate = true;
    }
     
    if(count == 0){
        free(text);
        return NULL;
    }
    return NULL;
}
  
void* consumer(void *arg){
    int idx = *(int*)arg;
    free(arg);
    struct timespec ts;
                                              ////
    while(!(isEmpty(lst) && end)) {
          
        if(clock_gettime(CLOCK_REALTIME, &ts) == -1) return NULL; //
        ts.tv_nsec += 10000*10000;                               //
        int ret = sem_timedwait(&sem, &ts);                       //
        if (ret == -1) continue;
          
        entry_t *element = pop(lst);
         
        if(element->num >= 0){
            if(element != NULL){
                pthread_mutex_lock(&mutex_print_Lock);
                printf("Thread %d:", idx);
                  
                for(int i = 0; i < element->num; i++){
                    if(i+1 == element->num){
                        printf(" %s\n", element->value);
                    }else{
                        printf(" %s", element->value);
                    }
                }
                 
                pthread_mutex_unlock(&mutex_print_Lock);
                free(element->value);
                free(element);
            }
        }else{
            free(element->value);
            free(element);
            negativ = true;
              
        }
    }
    return NULL;
}
  
int main(int argc, char* argv[]){
    // Check input argv
    int N = 1;
    if (argv[1] != NULL){
        N = atoi(argv[1]);
    }    
    if (N < 1 || N > sysconf(_SC_NPROCESSORS_ONLN)){
        return 1;
    }
     
    sem_init(&sem,0,0);
    pthread_t producent;
    pthread_t konzument[N];
    lst = listInit();
 
    pthread_mutex_init(&mutex_LL_Lock, NULL);
    pthread_mutex_init(&mutex_print_Lock, NULL);
  
    if(pthread_create(&producent,NULL, &producer, NULL) != 0){
        fprintf(stderr,"Failed to create thread in line%d\n", __LINE__);
    }
     
     for(int i=0; i < N; i++){
        int *arg = malloc(sizeof(*arg));
        *arg = i+1;
        if(pthread_create(&konzument[i],NULL, &consumer, arg) != 0){
            fprintf(stderr, "Failed to create thread in line %d\n", __LINE__);
        }
    }
     
    if(pthread_join(producent, NULL) != 0){
        fprintf(stderr, "Failed to join thread in line %d\n",__LINE__);
    }
    // for(int i=0; i<N;i++){
        // sem_post(&sem);
    // }
     
      
    for(int i=0; i < N; i++){
        if(pthread_join(konzument[i],NULL) != 0){
            fprintf(stderr, "Failed to join thread in line %d\n", __LINE__);
        }
    }
     
    sem_destroy(&sem);
    removeALL(lst);
    free(lst);
    pthread_mutex_destroy(&mutex_LL_Lock);
    pthread_mutex_destroy(&mutex_print_Lock);
    if(negativ || prod_terminate){
        return 1;
    }
    return 0;
}