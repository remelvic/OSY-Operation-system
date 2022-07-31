#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
 
int sig = 0;
void signal_handler(int signum){
    if (signum == SIGTERM){
        sig = 1;
    }
}
 
int main(){
    int fd[2];
    pid_t GEN;
    pid_t NSD;
    pid_t GEN_l;
    pid_t NSD_l;
    int status;
 
    if (pipe(fd) < 0) {
        exit(2);
    }
    if((GEN = fork())  == 0){ /* child procces */
        if(signal(SIGTERM, signal_handler) < 0) exit(2);
         
        if(close(fd[0]) < 0) exit(2);
        if(dup2(fd[1], STDOUT_FILENO) < 0) exit(2);
        if(close(fd[1]) < 0) exit(2);
        while(sig == 0){
            if(printf("%d %d\n", rand() % 4096, rand() % 4096) < 0) exit(2);
            if(fflush(stdout) < 0) exit(2);
            if(sleep(1) < 0) exit(2);
        }
        if(fprintf(stderr, "GEN TERMINATED\n") < 0) exit(2);
        exit(0);
    }else if(GEN < 0){
        exit(2);
    }
        if (GEN && (NSD = fork()) == 0){ /* child procces */
            if(close(fd[1]) < 0) exit(2);
            if(dup2(fd[0], STDIN_FILENO) < 0) exit(2);
            if(close(fd[0]) < 0) exit(2);
 
            if( execl("nsd", "nsd", NULL) < 0) exit(2);
        }else if(NSD < 0 && !GEN){
            exit(2);
        }
     
    if(close(fd[0]) < 0) exit(2);
    if(close(fd[1]) < 0) exit(2);
    if(sleep(5) < 0) exit(2);
     
    if(kill(GEN, SIGTERM) < 0) exit(2);
     
    if (waitpid(GEN, &status, 0) < 0) exit(2);
    GEN_l = WEXITSTATUS(status);
     
    if(waitpid(NSD, &status, 0) < 0) exit(2);
    NSD_l = WEXITSTATUS(status);
         
    if(GEN_l || NSD_l){
        printf("ERROR\n");
        exit(1);
    }else{
        printf("OK\n");
        exit(0);
    }
 
    return 1;
}