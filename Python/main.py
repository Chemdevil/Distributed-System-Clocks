from multiprocessing import Pipe, Process
from Services.LamportClock import process_a, process_b, process_c

from fastapi import FastAPI

from Services.VectorClock import vector_process_a, vector_process_b, vector_process_c

app = FastAPI()


@app.get("/")
async def root():
    return {"message": "Hello World"}


@app.get("/hello/{name}")
async def say_hello(name: str):
    return {"message": f"Hello {name}"}


@app.get("/lamportClock")
async def lamport_caller():
    a_and_b, b_and_a = Pipe()
    b_and_c, c_and_b = Pipe()
    processa = Process(target=process_a,args=(a_and_b,))
    processb = Process(target=process_b,args=(b_and_a,b_and_c))
    processc = Process(target=process_c,args=(c_and_b,))

    processa.start()
    processb.start()
    processc.start()

    processa.join()
    processb.join()
    processc.join()

@app.get("/vectorClock")
async def vector_caller():
    a_vectorand_b, b_vectorand_a = Pipe()
    b_vectorand_c, c_vectorand_b = Pipe()
    vectorprocessa = Process(target=vector_process_a,args=(a_vectorand_b,))
    vectorprocessb = Process(target=vector_process_b,args=(b_vectorand_a,b_vectorand_c))
    vectorprocessc = Process(target=vector_process_c,args=(c_vectorand_b,))

    vectorprocessa.start()
    vectorprocessb.start()
    vectorprocessc.start()

    vectorprocessa.join()
    vectorprocessb.join()
    vectorprocessc.join()

