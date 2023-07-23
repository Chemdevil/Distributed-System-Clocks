#include <string.h>
#include "LamportClock.h";
#include "stdio.h"
#include "unistd.h"
#include "time.h"

char* localTime(int counter){
    time_t now=time(NULL);
    char* timeStr = ctime(&now);
    printf("Lamport time=%d, LocalTime:%s", counter, asctime( localtime(&timeStr)));

}
int calculateRecvTimeStamp(int recvTimeStamp, int counter){
    return (recvTimeStamp>counter?recvTimeStamp:counter)+1;
}
int event(int processId, int counter){
    counter=counter+1;
    printf("Something happened in process: {%d}\n",processId);
    return counter;
};
int sendMessage(int* pipe, int processId, int counter){
    counter=counter+1;
    int message[2] = {0,counter};
    write(pipe[1],message,sizeof (message));
    char* currentTime = localTime(counter);
    printf("Message sent from {%d} %s\n",processId, &currentTime);
    return counter;
}
int receiveMessage(int* pipe, int processId, int counter){
    int message[2];
    read(pipe[0],message,sizeof(message));
    int senderTimeStamp = message[1];
    counter = calculateRecvTimeStamp(senderTimeStamp, counter);
    char* currentTime = localTime(counter);
    printf("Message received on {%d} %s\n",processId, &currentTime);
    return counter;
};
void process_a(int* pipeab){
    int pid= getpid();
    int counter=0;
    counter= event(pid, counter);
    counter= sendMessage(pipeab, pid, counter);
    counter= event(pid, counter);
    counter= receiveMessage(pipeab, pid, counter);
    counter= event(pid, counter);
    char* currentTime = localTime(counter);
    printf("Local time for process a {%d}, %s\n",counter, &currentTime);
};
void process_b(int* pipeba, int* pipebc){
    int processId=getpid();
    int counter=0;
    counter= receiveMessage(pipeba, processId, counter);
    counter= sendMessage(pipeba, processId, counter);
    counter= sendMessage(pipebc, processId, counter);
    counter= receiveMessage(pipebc, processId, counter);
    char* currentTime = localTime(counter);
    printf("Local time for process b {%d}, %s\n",counter, &currentTime);
};
void process_c(int* pipecb){
    int pid=getpid();
    int counter=0;
    counter= receiveMessage(pipecb, pid, counter);
    counter= sendMessage(pipecb, pid, counter);
    char* currentTime = localTime(counter);
    printf("Local time for process C {%d}, %s\n",counter, &currentTime);
};


