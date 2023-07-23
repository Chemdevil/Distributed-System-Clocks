//
// Created by shreyans on 11/7/23.
//

#ifndef UNTITLED_LAMPORTCLOCK_H
#define UNTITLED_LAMPORTCLOCK_H
char* localTime(int counter);
int calculateRecvTimeStamp(int recvTimeStamp, int counter);
int event(int processId, int counter);
int sendMessage(int* pipe, int processId, int counter);
int receiveMessage(int* pipe, int processId, int counter);
void process_a(int* pipeab);
void process_b(int* pipeba, int* pipebc);
void process_c(int* pipecb);

#endif //UNTITLED_LAMPORTCLOCK_H
