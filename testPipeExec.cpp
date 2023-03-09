#include "pipeExec.h"
#include "testPipeExec.h"
#include <chrono>
#include <unistd.h>

bool adder::run(void* data) {
	int* count;

	count = (int *)data;
	*count = *count + 1;

	return true;
}

bool subs::run(void* data) {
	int* count;

	count = (int *)data;
	*count = *count -  1;

	return true;
}

bool printer::run(void* data) {
	int* count;

	count = (int *)data;
	printf("Current count = %d\n", *count);

	return true;
}

adder * adder::clone() const { return new adder(); }
subs * subs::clone() const { return new subs(); }
printer * printer::clone() const { return new printer(); }



int  main(int argc, char** argv)
{

	adder   addOne;
	subs    subOne;
	printer printData;
	SimpleMemoryManager  *head;
	pipeExec *myPipe;
	int bufferSize = 10;


	//head = new SimpleMemoryManager(sizeof(int), 10);
	head = new SimpleMemoryManager(0, bufferSize);

	// Put the buffers in the free queue
	int *item;
	for (int i = 0; i < bufferSize; ++i)
	{
		item = (int *)malloc(sizeof(int));
		*item = 0;
		head->loadMemoryManager(item);

	}

	cout << "CREATING PIPE 1" << endl;
	myPipe = new pipeExec(&addOne, head);
	myPipe->addFunction(&subOne,1);
	myPipe->addFunction(&addOne,1);
	myPipe->addFunction(&subOne,1);
	myPipe->addFunction(&printData);

	cout << "RUNNING PIPE 1" << endl;
	myPipe->runPipe();


	int *data;
	for (int a = 0; a < 1; ++a)
		for (int i = 0; i < 20; ++i)
		{
			// Wait for a free buffer
			head->waitForFree();

			// Get the buffer
			data = (int*)head->getFreeBuffer();
			if (data == NULL ) {
				printf("GOT NULL !!! MAIN\n");
				break;
			}

			// Put the buffer i nthe proccessing queue
			head->putFullBuffer((void *)data);
		}

	head->waitForDone();

	myPipe->deleteFunction(&subOne);

     cout << "------------------------------- subOne removed -----------------------" << endl;

	for (int a = 0; a < 1; ++a)
		for (int i = 0; i < 20; ++i)
		{
			// Wait for a free buffer
			head->waitForFree();

			// Get the buffer
			data = (int*)head->getFreeBuffer();
			if (data == NULL ) {
				printf("GOT NULL !!! MAIN\n");
				break;
			}

			// Put the buffer i nthe proccessing queue
			head->putFullBuffer((void *)data);
		}

	// Wait for the head to be completly full to ensure all buffers have been processed !!!!!!!!!!!
	head->waitForDone();

	cout << "Free Count = " << head->getFreeCount() << " poolSize = " << head->getBufferCount() << endl;

	printf("Threads killed = %d\n", myPipe->killPipe());

	sleep(60);


	return 0;	
}
