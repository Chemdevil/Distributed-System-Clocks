#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "clockLibrary/Lamport/LamportClock.h"
#include "clockLibrary/Vector/VectorClock.h"
void LamportCaller(){
    int a_and_b[2];
    int b_and_a[2];
    int b_and_c[2];
    int c_and_b[2];
    pipe(a_and_b);
    pipe(b_and_a);
    pipe(b_and_c);
    pipe(c_and_b);
    pid_t processa = fork();
    if(processa){
//        if fork returns 0 process created
        close(a_and_b[0]);
        process_a(a_and_b);
    }
    pid_t processb=fork();
    if(processb){
        close(b_and_a[0]);
        close(b_and_c[0]);
        close(b_and_c[1]);
        process_b(b_and_a, b_and_c);
    }
    pid_t processc=fork();
    if(processc){
        close(c_and_b[1]);
        process_c(c_and_b);
    }

    close(a_and_b[0]);
    close(b_and_a[1]);
    close(b_and_c[0]);
    close(c_and_b[0]);
    close(c_and_b[1]);

    waitpid(processa,NULL,0);
    waitpid(processb,NULL,0);
    waitpid(processc,NULL,0);
}


void VectorCaller(){
    int a_and_b[2];
    int b_and_a[2];
    int b_and_c[2];
    int c_and_b[2];
    pipe(a_and_b);
    pipe(b_and_a);
    pipe(b_and_c);
    pipe(c_and_b);
    pid_t processa = fork();
    if(processa){
//        if fork returns 0 process created
        close(a_and_b[0]);
        vector_process_a(a_and_b);
    }
    pid_t processb=fork();
    if(processb){
        close(b_and_a[0]);
        close(b_and_c[0]);
//        close(b_and_c[1]);
        vector_process_b(b_and_a, b_and_c);
    }
    pid_t processc=fork();
    if(processc){
        close(c_and_b[1]);
        vector_process_c(c_and_b);
    }

    close(a_and_b[0]);
    close(b_and_a[1]);
    close(b_and_c[0]);
    close(c_and_b[0]);
    close(c_and_b[1]);

    waitpid(processa,NULL,0);
    waitpid(processb,NULL,0);
    waitpid(processc,NULL,0);
}

int main() {
    LamportCaller();
    VectorCaller();
}
