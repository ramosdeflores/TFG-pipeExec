CC=g++
DEPS = testPipeExec.h SimpleMemoryManager.h pipeExec.h
OBJ = testPipeExec.o SimpleMemoryManager.o pipeExec.o
CFLAGS=-lpthread

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

testPipeExec: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
