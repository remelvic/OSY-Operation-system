#include <cstdlib>
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
 
#include <set>
 
using namespace std;
 
class Node {
public:
    int priority;
    const char *work;
    bool outPeople;
    Node *next;
};
 
class List {
    Node *head;
    Node *tail;
    Node *temp;
    Node*lptr;
     
public:
    bool isEmpty(){
        return head == NULL;
    }
    List() {
        head = NULL;
        tail = NULL;
    }
 
    void insert(int x, const char* work) {
        temp = new Node;
        temp->priority = x;
        temp->work = work;
        temp->outPeople = false;
        if(isEmpty()){
            temp->next = NULL;
            tail = temp;
        }else{
            temp->next = head;
        }
        head = temp;
    }
 
    void removeFirst(){
        Node *temp = head;
        head = head->next;
        delete temp;
    }
 
    Node* find(int x) {
        temp = head;
        if(head == NULL){
            return NULL;
        }
        while(temp->priority != x){
            if(temp->next == NULL){
                return NULL;
            }else {
                temp = temp->next;
            }
        }
        return temp;
    }
 
    void display(){
        if(!isEmpty()){
            for(temp = head; temp != NULL; temp=temp->next)
                cout << temp->priority << " " << temp->work << " ";
            cout << endl;
        } else {
            //cout << "List is Empty!" << endl;
        }
    }
    void swap(Node *a, Node *b) {
        int temp = a->priority;
        const char* temp_work = a->work;
        bool temp_outPeople = a->outPeople;
        a->priority = b->priority;
        a->work = b->work;
        a->outPeople = b->outPeople;
        b->priority = temp;
        b->work = temp_work;
        b->outPeople = temp_outPeople;
    }
     
    void Sort(){
        int swapped;
         
        //Node *ptr1;
        lptr = NULL;
        if (head == NULL) return;
        do{
            swapped = 0;
            temp = head;
            //ptr1 = start;
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
     
};
 
class Node_N{
    public:
    const char* name;
    const char* work;
    Node_N *next;
};
 
 
class Args{
    public:
    char* name;
    char* job;
    //bool outWork;
};
 
List queue_A;
List queue_B;
List queue_C;
 
int error = 0;
int count = 0;
int countA = 1;     //controla vystupu v rutine 
int countB = 1;     //controla vystupu v rutine 
int countC = 1;     //controla vystupu v rutine 
int my_exit = 1;
int count_requestA = 0;
int count_requestB = 0;
int count_requestC = 0;
 
bool liveA = false;
bool endVojta = false;
bool endNora = false;
Node_N *LL = NULL;
int control = 0;
int controlA = 0;
int controlB = 0;
int controlC = 0;
//Args* args;
 
 
 
const char *arg;
pthread_mutex_t mutex;
 
 
pthread_cond_t work;
pthread_t worker;
 
void push(Node_N ** head_ref, char* new_name, const char* new_work){
    Node_N *new_node = new Node_N();
 
    new_node->name = new_name;
    new_node->work = new_work;
    new_node->next =(* head_ref);
    (*head_ref) = new_node;
     
}
 
void *routine (void *arg){
    Args *args = (Args*)arg;
    Node *foundA = NULL;
    Node *foundB = NULL;
    Node *foundC = NULL;
     
    if (endVojta || endNora){
        delete args;
        return (void*) 0;
    }
 
     
     
    while (1){
        label:
        if(controlA == 1){
            foundA = queue_A.find(countA);//find(countA, queue_A);
             
        }
        if(controlB == 1 ){
            foundB = queue_B.find(countB);//find(countB, queue_B);
        }
        if (controlC == 1){
            foundC = queue_C.find(countC);//find(countC, queue_queue_C);
        }
         
        pthread_mutex_lock(&mutex);
     
        while(control == 0 ){
            pthread_cond_wait(&work, &mutex);
        }
        pthread_mutex_unlock(&mutex);
 
        if(foundA != NULL && controlA != 0 ){
             
            if(strcmp(args->job, foundA->work) != 0 ){
                pthread_mutex_lock(&mutex);
                pthread_cond_wait(&work, &mutex);
                pthread_mutex_unlock(&mutex);
                 
                goto label;
                 
            }else{
                 
                printf("%s %s %d A\n", args->name, foundA->work,countA);
                 
                 
                if (strcmp(foundA->work, "nuzky") == 0){
                    usleep(100000);
                    if (liveA){
                        break;
                    }
                }else if (strcmp(foundA->work, "vrtacka") == 0){
                    usleep(200000);
                }else if (strcmp(foundA->work, "ohybacka") == 0){
                    usleep(150000);
                }else if (strcmp(foundA->work, "svarecka") == 0){
                    usleep(300000);
                }else if (strcmp(foundA->work, "lakovna") == 0) {
                    usleep(400000);
                }else if (strcmp(foundA->work, "sroubovak") == 0) {
                    usleep(250000);
                }else if (strcmp(foundA->work, "freza") == 0) {
                    usleep(500000);
                }
                if (strcmp(foundA->work, "lakovna") == 0 && countA == 6){
                    printf("done A\n");
                     
                }                
                 
            }
             
            pthread_mutex_lock(&mutex);
             
            queue_A.removeFirst();
            //queue_A.display();    
             
            countA+=1;
            pthread_cond_broadcast(&work);
            pthread_mutex_unlock(&mutex);
             
        }
        if(foundB != NULL && controlB != 0 && !foundB->outPeople){
            if(strcmp(args->job, foundB->work) != 0 ){
                pthread_mutex_lock(&mutex);
                pthread_cond_wait(&work, &mutex);
                pthread_mutex_unlock(&mutex);
                 
                goto label;
                 
            }else{
                 
                printf("%s %s %d B\n", args->name, foundB->work,countB);
                if (strcmp(foundB->work, "nuzky") == 0){
                    usleep(100000);
                }else if (strcmp(foundB->work, "vrtacka") == 0){
                    usleep(200000);
                }else if (strcmp(foundB->work, "ohybacka") == 0){
                    usleep(150000);
                }else if (strcmp(foundB->work, "svarecka") == 0){
                    usleep(300000);
                }else if (strcmp(foundB->work, "lakovna") == 0) {
                    usleep(400000);
                }else if (strcmp(foundB->work, "sroubovak") == 0) {
                    usleep(250000);
                }else if (strcmp(foundB->work, "freza") == 0) {
                    usleep(500000);
                }
                if(strcmp(foundB->work, "sroubovak") == 0 && countB == 6){
                    printf("done B\n");
                     
                }
                 
            }
            pthread_mutex_lock(&mutex);  
            queue_B.removeFirst();  
            countB+=1;
            pthread_cond_broadcast(&work);
            pthread_mutex_unlock(&mutex);
             
        }
         
        if(foundC != NULL && controlC != 0 && !foundC->outPeople){
            if(strcmp(args->job, foundC->work) != 0 ){
 
                pthread_mutex_lock(&mutex);
                pthread_cond_wait(&work, &mutex);
                pthread_mutex_unlock(&mutex);
                 
                goto label;
                 
            }else{
                printf("%s %s %d C\n", args->name, foundC->work,countC);
                if (strcmp(foundC->work, "nuzky") == 0){
                    usleep(100000);
                }else if (strcmp(foundC->work, "vrtacka") == 0){
                    usleep(200000);
                }else if (strcmp(foundC->work, "ohybacka") == 0){
                    usleep(150000);
                }else if (strcmp(foundC->work, "svarecka") == 0){
                    usleep(300000);
                }else if (strcmp(foundC->work, "lakovna") == 0) {
                    usleep(400000);
                }else if (strcmp(foundC->work, "sroubovak") == 0) {
                    usleep(250000);
                }else if (strcmp(foundC->work, "freza") == 0) {
                    usleep(500000);
                }
                if(strcmp(foundC->work, "lakovna") == 0 && countC == 6){
                    printf("done C\n");
                }
            }
            pthread_mutex_lock(&mutex);    
            countC+=1;
            pthread_cond_broadcast(&work);
            pthread_mutex_unlock(&mutex);
        }
         
        //TODO: чекать если массив пожадавку пустой но там есть число 5 например вртачка в А
        if(!queue_A.isEmpty() && !queue_B.isEmpty() && (foundA != NULL || foundB != NULL)){
           break;
        }
 
        if(foundA == NULL && controlA != 0 && foundC == NULL && foundB == NULL && controlB == 0 && controlC == 0){
            // printf("1");
            break;
        } // only A error input
        if(foundA == NULL && controlB != 0 && foundC == NULL && foundB == NULL && controlC == 0 && controlA == 0){
            //printf("2");
            break;
        } // only B error input
        if(foundA == NULL && controlC != 0 && foundC == NULL && foundB == NULL && controlA == 0 && controlB == 0){
            //printf("4");
            break;
        } // only C error input
        if (foundA == NULL && controlA != 0 && controlB != 0 && foundC == NULL && foundB == NULL && controlC == 0){
            //printf("5");
            //break;
        } // only A and B error input
        if (foundA == NULL && controlA != 0 && controlC != 0 && foundC == NULL && foundB == NULL && controlB == 0){
            //printf("6");
            break;
        } // only A and C error input
        if(foundA == NULL && controlB != 0 && controlC != 0 && foundC == NULL && foundB == NULL && controlA == 0){
            //printf("7");
            break;
        }// only B and C error input
        if(foundA == NULL && controlA != 0 && controlB != 0 && controlC != 0 && foundC == NULL && foundB == NULL){
            //printf("8"); 
            break;
        }
    }
     
    delete args;
    return (void*) 0;
}
 
int main(int argc, char ** argv){
     
    pthread_cond_init(&work, NULL);
    pthread_mutex_init(&mutex, NULL);
     
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
            Args* argForWork = new Args();
            if (strlen(arg1) > 12) exit(0); 
            char *name = strdup(arg1);
            char *workArg = strdup(arg2);
             
            pthread_mutex_lock(&mutex);
            push(&LL,name,workArg);
            argForWork->name = name;
            argForWork->job = workArg;
            //argForWork->outWork = false;
 
            pthread_mutex_unlock(&mutex);
 
            if (strcmp(workArg, "vrtackaX") == 0){
                liveA = true;
                //break;
                continue;
            }
            if(pthread_create(&worker, NULL, routine, (void*) argForWork) != 0){
                fprintf(stderr, "failed to create in line %d\n",__LINE__);
            }
             
            count+=1;
              
        } else if (strcmp(cmd, "make") == 0 && arg1 && !arg2) {
             
            if (strcmp(arg1, "A") == 0){
                pthread_mutex_lock(&mutex);
                controlA = 1;
                control = 1;
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "B") == 0){
                pthread_mutex_lock(&mutex);
                controlB = 1;
                control = 1;
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "C") == 0){
                pthread_mutex_lock(&mutex);
                controlC = 1;
                control = 1;
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
            }
             
        } else if (strcmp(cmd, "end") == 0 && arg1 && !arg2) {
             
             
            if (strcmp(arg1, "Vojta") == 0){
                pthread_mutex_lock(&mutex);    
                endVojta = true;
                // queue_A.find(2)->outPeople = true;
                // queue_A.find(5)->outPeople = true;
                // queue_B.find(1)->outPeople = true;
                // queue_B.find(4)->outPeople = true;
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "Nora") == 0){
                pthread_mutex_lock(&mutex);
                endNora = true;
                // queue_A.find(1)->outPeople = true;
                // queue_B.find(2)->outPeople = true;
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
            }
             
            /* tell the worker to finish
             * the worker has to finish their work first
             * you should not wait here for the worker to finish
             *
             * if the worker is waiting for work
             * you need to wakeup the worker
             */
        } else if (strcmp(cmd, "add") == 0 && arg1 && !arg2) {
             
            if(strcmp(arg1, "nuzky") == 0){
                arg = "nuzky";
                pthread_mutex_lock(&mutex);
                 
                queue_A.insert(1, arg);
                //push(&queue_A,1,arg);
                //push(&queue_B,2,arg);
                queue_B.insert(2, arg);
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
            }else if (strcmp(arg1,"vrtacka") == 0){
                arg = "vrtacka";
                pthread_mutex_lock(&mutex);
                queue_A.insert(2, arg);
                queue_A.insert(5, arg);
                queue_B.insert(1, arg);
                queue_B.insert(4, arg);
                queue_C.insert(2, arg);
                queue_C.insert(4, arg);
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
                // push(&queue_A,2,arg);
                // push(&queue_A,5,arg);
                // push(&queue_B,1,arg);
                // push(&queue_B,4,arg);
                // push(&queue_C,2,arg);
                // push(&queue_C,4,arg);
            }else if (strcmp(arg1, "ohybacka") == 0){
                arg = "ohybacka";
                pthread_mutex_lock(&mutex);
                queue_A.insert(3,arg);
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
                //push(&queue_A,3,arg);
            }else if (strcmp(arg1, "svarecka") == 0){
                arg = "svarecka";
                pthread_mutex_lock(&mutex);
                //push(&queue_A,4,arg);
                queue_A.insert(4, arg);
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
            }else if(strcmp(arg1, "lakovna") == 0){
                arg = "lakovna";
                pthread_mutex_lock(&mutex);
                queue_A.insert(6,arg);
                queue_B.insert(5,arg);
                queue_C.insert(6,arg);
 
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
                // push(&queue_A,6,arg);
                // push(&queue_B,5,arg);
                // push(&queue_C,6,arg);
            }else if(strcmp(arg1 , "freza") == 0){
                arg = "freza";
                pthread_mutex_lock(&mutex);
                queue_B.insert(3,arg);
                queue_C.insert(1,arg);
                queue_C.insert(5,arg);
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
                // push(&queue_B,3,arg);
                // push(&queue_C,1,arg);
                // push(&queue_C,5,arg);
            }else if(strcmp(arg1 , "sroubovak") == 0){
                arg = "sroubovak";
                pthread_mutex_lock(&mutex);
                queue_B.insert(6,arg);
                queue_C.insert(3,arg);
                pthread_cond_broadcast(&work);
                pthread_mutex_unlock(&mutex);
                // push(&queue_B,6,arg);
                // push(&queue_C,3,arg);    
            }
             
            /* add new place
             *
             * if worker and part is ready, start working - wakeup worker
             */
            queue_A.Sort();
            queue_B.Sort();
            queue_C.Sort();
            // Sort(queue_A);
            // Sort(queue_B);
            // Sort(queue_C);
        } else if (strcmp(cmd, "remove") == 0 && arg1 && !arg2) {
            /* if you cannot remove empty place you cannot wait for finish
             * work
             */
        } else {
            fprintf(stderr, "Invalid command: %s\n", line);
        }
         
        free(line);
             
    }
 
    for(int i = 0; i < count; i++){
         
        pthread_join(worker, NULL);
        pthread_exit(NULL);
    }
 
    pthread_cond_destroy(&work);
    pthread_mutex_destroy(&mutex);
     
      
    // deleteList(queue _A);
    // deleteList(queue_B);
    // deleteList(queue_C);
     
    return 0;
}