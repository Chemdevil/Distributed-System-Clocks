//
// Created by shreyans on 12/7/23.
//

#ifndef UNTITLED_VECTORCLOCK_H
#define UNTITLED_VECTORCLOCK_H
char* vectorlocalTime(int* counter);
int* vectorcalculateRecvTimeStamp(int* recvTimeStamp, int* counter,  int size);
int* vector_event(int processId, int* counter);
int* vector_send_Message(int* pipe, int processId, int* counter);
int* vector_receive_Message(int* pipe, int processId, int* counter, int size);
void vector_process_a(int* pipeab);
void vector_process_b(int* pipeba, int* pipebc);
void vector_process_c(int* pipecb);
#endif //UNTITLED_VECTORCLOCK_H
