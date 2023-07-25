from os import getpid
from datetime import datetime


def localtime(counter):
    return '(lamport time={}, Local_Time={})'.format(counter, datetime.now())


def calculaterecvtimestamp(recvTimeStamp, counter):
    return max(recvTimeStamp, counter) + 1


def event(processId, counter):
    counter += 1
    print('Something happened in {} !'. \
          format(processId) + localtime(counter))
    return counter


def sendmessage(pipe, processId, counter):
    counter += 1
    pipe.send(('Empty shell', counter))
    print('Message sent from ' + str(processId) + localtime(counter))
    return counter


def receive_message(pipe, processId, counter):
    message, sendertimestamp = pipe.recv()
    counter = calculaterecvtimestamp(sendertimestamp, counter)
    print("Message received at " + str(processId) + localtime(counter))
    return counter


def process_a(pipe_ab):
    pid = getpid()
    counter = 0
    counter = event(pid, counter)
    counter = sendmessage(pipe_ab, pid, counter)
    counter = event(pid, counter)
    counter = receive_message(pipe_ab, pid, counter)
    counter = event(pid, counter)


def process_b(pipe_ab, pipe_bc):
    pid = getpid()
    counter = 0
    counter = receive_message(pipe_ab, pid, counter)
    counter = sendmessage(pipe_ab, pid, counter)
    counter = sendmessage(pipe_bc, pid, counter)
    counter = receive_message(pipe_bc, pid, counter)


def process_c(pipe_bc):
    pid = getpid()
    counter = 0
    counter = receive_message(pipe_bc, pid, counter)
    counter = sendmessage(pipe_bc, pid, counter)

