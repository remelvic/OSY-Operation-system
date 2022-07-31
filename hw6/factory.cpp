#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

class structForWorker_t{
public:
    char* name;
    char* job;
    pthread_t thread;
    bool outWork;
};

enum place {
    NUZKY, VRTACKA, OHYBACKA, SVARECKA, LAKOVNA, SROUBOVAK, FREZA,
    _PLACE_COUNT
};

const char *place_str[_PLACE_COUNT] = {
    [NUZKY] = "nuzky",
    [VRTACKA] = "vrtacka",
    [OHYBACKA] = "ohybacka",
    [SVARECKA] = "svarecka",
    [LAKOVNA] = "lakovna",
    [SROUBOVAK] = "sroubovak",
    [FREZA] = "freza",
};

enum product {
    A, B, C,
    _PRODUCT_COUNT
};

const char *product_str[_PRODUCT_COUNT] = {
    [A] = "A",
    [B] = "B",
    [C] = "C",
};

int find_string_in_array(const char **array, int length, char *what)
{
    for (int i = 0; i < length; i++)
        if (strcmp(array[i], what) == 0)
            return i;
    return -1;
}

/* It is not necessary to represent each working place with a dynamically
 * allocated object.  You can store only number of ready places
 *
 * int ready_places[_PLACE_COUNT];
 */

/* It is not necessary to represent each part as a dynamically allocated
 * object.  you can have only number of parts for each working phase
 */

#define _PHASE_COUNT 6
int parts[_PRODUCT_COUNT][_PHASE_COUNT] = {0};
 
void find_time(char* time){
    if (strcmp(time,"nuzky") == 0){
        usleep(100000);
    }else if(strcmp(time, "vrtacka") == 0){
        usleep(200000);
    }else if(strcmp(time, "ohybacka") == 0){
        usleep(150000);
    }else if(strcmp(time, "svarecka") == 0){
        usleep(300000);
    }else if (strcmp(time, "lakovna") == 0){
        usleep(400000);
    }else if (strcmp(time, "sroubovak") == 0){
        usleep(250000);
    }else if (strcmp(time, "freza") == 0){
        usleep(500000);
    }
}

class Node{
public:
    int priority;
    const char* work;
    int end_work;
    //bool outPeople
    Node *next;
};

class LinkedList{
    Node *head;
    Node *tail;
    Node *temp;
    Node *lptr;
public:
    bool isEmpty(){
        return head == NULL;
    }
    LinkedList(){
        head = NULL;
        tail = NULL;
    }
    void insert(int priority, const char* work){
        temp = new Node;
        temp->priority = priority;
        temp->work = work;
        if(isEmpty()){
            temp->next = NULL;
            tail = temp;
        }else{
            temp->next = head;
        }
        head = temp;
    }
    
   void removeNode(){
       Node *temp = head;
       head = head->next;
       delete temp;
   }
   
   Node* find(int priority){
       temp = head;
       if(head == NULL){
           return NULL;
       }
       while(temp->priority != priority){
           if(temp->next == NULL){
               return NULL;
           }else{
               temp = temp->next;
           }
       }
       return temp;
   }
   int findByName(char* job){
       temp = head;
       if (head == NULL){
           return 0;
       }
       while(strcmp(job, temp->work) != 0){
           if(temp->next == NULL){
               return 0;
           }else{
               temp = temp->next;
           }
       }
       return temp->priority;

   }

    void display(){
        if(!isEmpty()){
            for(temp = head; temp != NULL; temp = temp->next){
                cout << temp->priority << " " << temp->work << " ";
            }
            cout << endl;
        }
    }

    void swap(Node *a, Node *b){
        int temp = a->priority;
        const char* temp_work = a->work;
        a->priority = b->priority;
        a->work = b->work;
        b->priority = temp;
        b->work = temp_work;
    }

    void Sort(){
        int swapped;

        lptr = NULL;
        if(head == NULL) return;
        do{
            swapped = 0;
            temp = head;
            while(temp->next != lptr){
                if(temp->priority > temp->next->priority){
                    swap(temp,temp->next);
                    swapped = 1;
                }
                temp = temp->next;
            }
            lptr = temp;
        }while(swapped);

    }
    void deleteNodeIndex(int priority){
        temp = head;
        Node* prev;
        if(temp == NULL){
            return;
        }
        if(priority == 1){
            head = temp->next;
            delete temp;
            return;
        }
        for(int i = 1; i != priority; i++){
            prev = temp;
            temp = temp->next;
        }
        if(temp == NULL){
            return;
        }else{
            prev->next = temp->next;
            delete temp;
        }

    }
    void deleteLinkedList(){
        Node *curr = head;
        Node *next;
        while(curr != NULL){
            next = curr->next;
            delete curr;
            curr = next;
        }
        head = NULL;
    }

};

LinkedList place_workA;
LinkedList place_workB;
LinkedList place_workC;


int curr_productA = 1;
int curr_productB = 1;
int curr_productC = 1;
const char *arg;
pthread_cond_t work;
pthread_mutex_t mutex;
int count_for_thread = 0;

structForWorker_t *my_worker = NULL;

void *routine (void *arg){
    int idx = (intptr_t) arg;
    
    int num_productA = place_workA.findByName(my_worker[idx].job);
    int num_productB = place_workB.findByName(my_worker[idx].job);
    int num_productC = place_workC.findByName(my_worker[idx].job);
    while(1){

        pthread_mutex_lock(&mutex);
        if(parts[0][0]){
            if(num_productA != 0 && idx >= 0 && place_workA.findByName(my_worker[idx].job) && curr_productA == num_productA){
                printf("%s %s %d A\n",my_worker[idx].name, my_worker[idx].job, num_productA);
                find_time(my_worker[idx].job);
                curr_productA++;
                pthread_cond_broadcast(&work);
            }
            if (curr_productA == 7){
                printf("done A\n");
                
            }
        }else if(parts[0][1]){
            if(num_productB != 0 && idx >= 0 && place_workB.findByName(my_worker[idx].job) && curr_productB == num_productB){
                printf("%s %s %d B\n",my_worker[idx].name, my_worker[idx].job, num_productB);
                find_time(my_worker[idx].job);
                curr_productB++;
                pthread_cond_broadcast(&work);
            }
            if (curr_productB == 7){
                printf("done B\n");
                
            }
        }else if(parts[0][2]){
            if(num_productC != 0 && idx >= 0 && place_workC.findByName(my_worker[idx].job) && curr_productC == num_productC){
                printf("%s %s %d C\n",my_worker[idx].name, my_worker[idx].job, num_productC);
                find_time(my_worker[idx].job);
                curr_productC++;
                pthread_cond_broadcast(&work);
            }
            if (curr_productC == 7){
                printf("done C\n");
                
            }
        }else {
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&work, &mutex);
            pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_unlock(&mutex);
        
        
    }
    
    return NULL;
        
        //process after work -- done product_type

        // exit    
}


int main(int argc, char **argv)
{
    pthread_cond_init(&work,NULL);
    pthread_mutex_init(&mutex,NULL);
    
    while (1) {
        char *line, *cmd, *arg1, *arg2, *arg3, *saveptr;
        int s = scanf(" %m[^\n]", &line);
        if (s == EOF)
            break;
        if (s == 0)
            continue;

        cmd  = strtok_r(line, " ", &saveptr);
        arg1 = strtok_r(NULL, " ", &saveptr);
        arg2 = strtok_r(NULL, " ", &saveptr);
        arg3 = strtok_r(NULL, " ", &saveptr);

        if (strcmp(cmd, "start") == 0 && arg1 && arg2 && !arg3) {
            pthread_mutex_lock(&mutex);
            my_worker = (structForWorker_t*)realloc(my_worker, (count_for_thread+1) * sizeof(structForWorker_t));
            //TODO: check strlen(arg1) > 12 exit(0)
            //TODO: check (strcmp(arg2, "vrtackaX") == 0)
            my_worker[count_for_thread].name = strdup(arg1);
            my_worker[count_for_thread].job = strdup(arg2);
            my_worker[count_for_thread].outWork = false;

            pthread_create(&my_worker[count_for_thread].thread, NULL, &routine, (void*)(intptr_t)count_for_thread);
            
            count_for_thread+=1;
            
            pthread_mutex_unlock(&mutex);
            pthread_cond_broadcast(&work);

        } else if (strcmp(cmd, "make") == 0 && arg1 && !arg2) {
            if(strcmp(arg1, "A") == 0){
                pthread_mutex_lock(&mutex);
                parts[0][0]++;
                pthread_mutex_unlock(&mutex);
            
            }else if(strcmp(arg1, "B") == 0){
                pthread_mutex_lock(&mutex);
                parts[1][0]++;
                pthread_mutex_unlock(&mutex);
            
            }else if (strcmp(arg1, "C") == 0){
                pthread_mutex_lock(&mutex);
                parts[2][0]++;
                pthread_mutex_unlock(&mutex);
            }
            pthread_cond_broadcast(&work);
            
        } else if (strcmp(cmd, "end") == 0 && arg1 && !arg2) {
            for(int i = 0; i < count_for_thread; i++){
                if(strcmp(arg1, my_worker[i].name) ==0){
                    pthread_mutex_lock(&mutex);
                    my_worker[i].outWork = true;
                    pthread_mutex_lock(&mutex);
                    pthread_cond_broadcast(&work);
                    break;
                }
            }
        
        } else if (strcmp(cmd, "add") == 0 && arg1 && !arg2) {
            if(strcmp(arg1, "nuzky") == 0){
                arg = "nuzky";
                pthread_mutex_lock(&mutex);
                place_workA.insert(1,arg);
                place_workB.insert(2,arg);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "vrtacka") == 0){
                arg = "vrtacka";
                pthread_mutex_lock(&mutex);
                place_workA.insert(2,arg);
                place_workA.insert(5,arg);
                place_workB.insert(1,arg);
                place_workB.insert(4,arg);
                place_workC.insert(2,arg);
                place_workC.insert(4,arg);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "ohybacka") == 0){
                arg = "ohybacka";
                pthread_mutex_lock(&mutex);
                place_workA.insert(3,arg);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1,"svarecka") == 0){
                arg = "svarecka";
                pthread_mutex_lock(&mutex);
                place_workA.insert(4,arg);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1,"lakovna") == 0){
                arg = "lakovna";
                pthread_mutex_lock(&mutex);
                place_workB.insert(5,arg);
                place_workA.insert(6,arg);
                place_workC.insert(6,arg);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1,"sroubovak") == 0){
                arg = "sroubovak";
                pthread_mutex_lock(&mutex);
                place_workB.insert(6,arg);
                place_workC.insert(3,arg);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "freza") == 0){
                arg = "freza";
                pthread_mutex_lock(&mutex);
                place_workB.insert(3,arg);
                place_workC.insert(1,arg);
                place_workC.insert(5,arg);
                pthread_mutex_unlock(&mutex);
            }
            place_workA.Sort();
            place_workB.Sort();
            place_workC.Sort();
            pthread_cond_broadcast(&work);
        } else if (strcmp(cmd, "remove") == 0 && arg1 && !arg2) {
            
            if(strcmp(arg1, "nuzky") == 0){
                arg = "nuzky";
                pthread_mutex_lock(&mutex);
                place_workA.deleteNodeIndex(1);
                place_workB.deleteNodeIndex(2);

                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "vrtacka") == 0){
                arg = "vrtacka";
                pthread_mutex_lock(&mutex);
                place_workA.deleteNodeIndex(2);
                place_workA.deleteNodeIndex(5);
                place_workB.deleteNodeIndex(1);
                place_workB.deleteNodeIndex(4);
                place_workC.deleteNodeIndex(2);
                place_workC.deleteNodeIndex(4);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "ohybacka") == 0){
                arg = "ohybacka";
                pthread_mutex_lock(&mutex);
                place_workA.deleteNodeIndex(3);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg, "svarecka") == 0){
                arg = "svarecka";
                pthread_mutex_lock(&mutex);
                place_workA.deleteNodeIndex(4);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "lakovna") ==0 ){
                arg = "lakovna";
                pthread_mutex_lock(&mutex);
                place_workA.deleteNodeIndex(6);
                place_workB.deleteNodeIndex(5);
                place_workC.deleteNodeIndex(6);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "sroubovak") ==0 ){
                arg = "sroubovak";
                pthread_mutex_lock(&mutex);
                place_workB.deleteNodeIndex(6);
                place_workC.deleteNodeIndex(3);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "freza") == 0){
                arg = "freza";
                pthread_mutex_lock(&mutex);
                place_workB.deleteNodeIndex(3);
                place_workC.deleteNodeIndex(1);
                place_workC.deleteNodeIndex(5);
                pthread_mutex_unlock(&mutex);
            }
            //pthread_cond_broadcast(&work);
        } else {
            fprintf(stderr, "Invalid command: %s\n", line);
        }
        
        free(line);
    }

    //free memory and join thread
    place_workA.deleteLinkedList();
    place_workB.deleteLinkedList();
    place_workC.deleteLinkedList();
    
    for(int i = 0; i < count_for_thread; i++){
        pthread_join(my_worker[i].thread, NULL);
        printf("Thread join %s", my_worker[i].name);
    }
    for(int i = 0; i < count_for_thread; i++){
        free(my_worker[i].name);
        free(my_worker[i].job);
    }
    free(my_worker);
    
    return 0;
}