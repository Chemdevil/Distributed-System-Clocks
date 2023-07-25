from datetime import datetime


def localtime(counter):
    return '(Vector time={}, Local_Time={})'.format(counter, datetime.now())


def calculaterecvtimestamp(recvTimeStamp, counter):
    for id in range(len(counter)):
        counter[id] = max(recvTimeStamp[id], counter[id])
    return counter


def event(processId, counter):
    counter[processId] = counter[processId] + 1
    print(f"Something happened in {processId} at {counter}")
    return counter


def sendmessage(pipe, processId, counter):
    counter[processId] = counter[processId] + 1
    pipe.send(("Message", counter))
    print("Message sent from " + str(processId) + localtime(counter))
    return counter


def receive_message(pipe, processId, counter):
    message, sendertimestamp = pipe.recv()
    counter = calculaterecvtimestamp(sendertimestamp, counter)
    print("Message received at " + str(processId) + localtime(counter))
    return counter


def vector_process_a(pipe_ab):
    pid = 0
    counter = [0, 0, 0]
    counter = event(pid, counter)
    counter = sendmessage(pipe_ab, pid, counter)
    counter = event(pid, counter)
    counter = receive_message(pipe_ab, pid, counter)
    counter = event(pid, counter)


def vector_process_b(pipe_ab, pipe_bc):
    pid = 1
    counter = [0, 0, 0]
    counter = receive_message(pipe_ab, pid, counter)
    counter = sendmessage(pipe_ab, pid, counter)
    counter = sendmessage(pipe_bc, pid, counter)
    counter = receive_message(pipe_bc, pid, counter)


def vector_process_c(pipe_bc):
    pid = 2
    counter = [0, 0, 0]
    counter = receive_message(pipe_bc, pid, counter)
    counter = sendmessage(pipe_bc, pid, counter)
