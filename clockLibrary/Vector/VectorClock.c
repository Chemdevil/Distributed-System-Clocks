#include "VectorClock.h"
#include "stdio.h"
#include "time.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"

int* messageParser(char* data){
    static int c[3]={-1, -1, -1};
    int startArrayIndex=0;
    int endArrayIndex=0;
    for(int i=0;i<strlen(data);i++){
        if(data[i]=='['){
            startArrayIndex=i+1;
        }else if(data[i]==']'){
            endArrayIndex=i;
            break;
        }
    }
    char substring[endArrayIndex-startArrayIndex];
    int start=0;
    strncpy(substring,data+startArrayIndex,endArrayIndex-startArrayIndex);
    char *token=strtok(substring,",");
    start=0;
    while(token!=NULL){
        c[start]=atoi(token);
        token = strtok(NULL, ",");
        start++;
    }
    return c;

}
void swapCounter(int *old, int *new,int size) {
    int temp[size];
    for(int index=0;index< (size);index++){
        temp[index]=old[index];
        old[index]=new[index];
        new[index]=temp[index];
    }
}

char* vectorlocalTime(int* counter){
    time_t now=time(NULL);
    char* timeStr = ctime(&now);
    char *lt = asctime( localtime(&timeStr));
    char timeStrCounter[300];
    sprintf(timeStrCounter,"Vector Clock = {[%d,%d,%d]}, LocalTime:%s",counter[0],counter[1],counter[2], lt);
    return lt;
}

int* vectorcalculateRecvTimeStamp(int* recvTimeStamp, int* counter,  int size){
    for(int i=0;i<size;i++){
        counter[i]=(recvTimeStamp[i]>counter[i])?recvTimeStamp[i]:counter[i];
    }
    return counter;
}

int* vector_event(int processId, int* counter){
    counter[processId]+=1;
    char* localtimeString = vectorlocalTime(counter);
    printf("Vector Clock = {[%d,%d,%d]}, LocalTime:%s",counter[0],counter[1],counter[2], localtimeString);
    return counter;
}
int* vector_send_Message(int* pipe, int processId, int* counter){
    counter[processId]+=1;
    char sendMessageData[100];
    sprintf(sendMessageData,"0-[%d,%d,%d]",counter[0],counter[1],counter[2]);
    write(pipe[1],sendMessageData,sizeof (sendMessageData));
    char* timeData = vectorlocalTime(counter);
    printf("Sending vector message from processId {%d}, at {%s}",processId,timeData);
    return counter;
}
int* vector_receive_Message(int* pipe, int processId, int* counter, int size){
    char receiveMessage[100];
    read(pipe[0],receiveMessage, sizeof(receiveMessage));
    int* receiveMessageData;
    receiveMessageData=messageParser(receiveMessage);
    counter= vectorcalculateRecvTimeStamp(receiveMessageData,counter,3);
    char* timeData = vectorlocalTime(counter);
    printf("Receiving vector message from processId {%d}, at {%s}",processId,timeData);
    return counter;
}
void vector_process_a(int* pipeab) {
    int pid=0;
    int counter[3]={0,0,0};
    swapCounter(counter, vector_event(pid, counter), 3);
    vector_send_Message(pipeab, pid, counter);
    swapCounter(counter, vector_event(pid, counter), 3);
    char* localtimeString = vectorlocalTime(counter);
    printf("Vector Clock at a= {[%d,%d,%d]}, LocalTime:%s",counter[0],counter[1],counter[2], localtimeString);
}

void vector_process_b(int* pipeba, int* pipebc){
    int pid=1;
    int counter[3]={0,0,0};
    swapCounter(counter, vector_receive_Message(pipeba, pid, counter, 3), 3);
    swapCounter(counter, vector_send_Message(pipeba, pid, counter), 3);
    swapCounter(counter, vector_send_Message(pipebc, pid, counter), 3);
    swapCounter(counter, vector_receive_Message(pipebc, pid, counter, 3), 3);
    char* localtimeString = vectorlocalTime(counter);
    printf("Vector Clock at b= {[%d,%d,%d]}, LocalTime:%s",counter[0],counter[1],counter[2], localtimeString);
}
void vector_process_c(int* pipecb){
    int pid=2;
    int counter[3]={0,0,0};
    swapCounter(counter, vector_receive_Message(pipecb, pid, counter, 3), 3);
    swapCounter(counter, vector_send_Message(pipecb, pid, counter), 3);
    char* localtimeString = vectorlocalTime(counter);
    printf("Vector Clock at c = {[%d,%d,%d]}, LocalTime:%s",counter[0],counter[1],counter[2], localtimeString);
}

