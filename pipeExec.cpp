#include "pipeExec.h"
#include <ostream>

// For use in deleteNode. Does nothing.
class nullFunc : public PipeBase {
	   nullFunc * clone() const {
			 return new nullFunc();
	   }
};

// Create 
//pipeExec::pipeExec(pipeExecFunc func, SimpleMemoryManager* mgrIn, int instances)
pipeExec::pipeExec(PipeBase *func, SimpleMemoryManager* mgrIn, int instances) {
	   pipeExecArgs *element;

	   // Create the HEAD node
	   element = (pipeExecArgs*) malloc(sizeof(pipeExecArgs));
	   element->instances = instances;
	   element->currentInstance = 0;
	   element->procFunc = func;
	   element->mgrIn  = mgrIn;
	   element->mgrOut = new SimpleMemoryManager(0, mgrIn->getBufferCount());
	   element->isTail = true;
	   element->switching = false;
	   element->deleted = false;
	   execList.push_back(element);
	   count = 0;
}

pipeExec::~pipeExec() {
	   int i;

	   killPipe(); // Destroy all the threads

	   // Free the elements in the vector. The last mgrOut is the first mgrIn
	   // and should be deleted by the calling function where it has been
	   // created
	   for ( i = 0; i < execList.size() - 1; ++i) {
			 delete execList[i]->mgrOut;
			 free(execList[i]);
	   }

	   free(execList[i]);
}

//void pipeExec::addFunction(pipeExecFunc func, int instances)
void pipeExec::addFunction(PipeBase *func, int instances, bool splice) {
	   pipeExecArgs *element;

	   // Insert an element
	   element = (pipeExecArgs*) malloc(sizeof(pipeExecArgs));
	   element->instances = instances;
	   element->currentInstance = 0;
	   element->procFunc = func;
	   execList[count]->mgrOut = new SimpleMemoryManager(0, execList[0]->mgrIn->getBufferCount());
	   element->mgrIn  = execList[count]->mgrOut;
	   element->mgrOut = execList[0]->mgrIn;
	   execList[count]->isTail = false;
	   element->isTail = true;
	   element->switching = false;
	   element->deleted = false;
	   execList.push_back(element);
	   ++count;
}

static std::mutex launchMutex_;

void execElement(pipeExec::pipeExecArgs* localArgs) {
	   int id;

	   id = localArgs->threadId;

	   try {
			 bool cont;
			 void* data;
			 PipeBase *localFunc;

			 if ( localArgs->currentInstance != 0 )
				    localFunc = localArgs->procFunc->clone(); // Get a new instance
			 else
				    localFunc = localArgs->procFunc;

			 // Needed do to a race condition with currentInstance
			 launchMutex_.unlock();

			 cont = localFunc->init();

			 while ( cont ) {
				    localArgs->mgrIn->waitForFull();
				    data = localArgs->mgrIn->getFullBuffer();

				    if ( data == (void*)NULL ) break; // Terminate

				    if ( localArgs->switching ) localArgs->stop.lock();
				    cont = localFunc->run((void*)data);

				    if ( ! localArgs->isTail )
						  localArgs->mgrOut->putFullBuffer(data);
				    else
						  localArgs->mgrOut->putFreeBuffer(data);
				    if ( localArgs->switching ) localArgs->stop.unlock();

				    // If switched, then init fuction has to be called again
				    if ( localArgs->switching )
				    { localArgs->stop.lock();
						  cont = localFunc->init();
						  localArgs->stop.unlock();
						  localArgs->switching = false;
				    }
			 }

			 localFunc->end();

	   } catch(...) {
			 cout << "execElement() - EXCEPTION CAUGHT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			 throw;
	   }

	   return;
}

int pipeExec::runPipe()
{
	   int execCount = 0;

	   for ( int i0 = 0; i0 < execList.size(); ++i0) {
			 //			 cout << "Launching " << execList[i0]->instances << " instances" << endl;
			 for ( int i1 = 0; i1 < execList[i0]->instances; ++i1 ) {
				    // Needed do to a race condition with currentInstance - see execElement
				    launchMutex_.lock();
				    execList[i0]->currentInstance = i1;
				    try {
						  execList[i0]->runningThreads.push_back( new std::thread(execElement, execList[i0]));
				    } catch(...) {
						  cout << "runPipe() - Error creating thread" << endl;
						  throw;
				    }
				    execList[i0]->threadId = execCount++;
			 }
	   }

	   return execCount;
}

// Can not destroy de node because there are other threads waiting on the pipe
// We just destroy all the threads and add a null function to allow for the
// data to pass thru
void pipeExec::deleteNode(int index) {

	   killNode(index);

	   execList[index]->procFunc = new nullFunc();
	   execList[index]->instances = 1;
	   execList[index]->deleted = true;
	   execList[index]->runningThreads.push_back(new std::thread(execElement, execList[index]));
}

int pipeExec::killNode(int index) {

	   //	   cout << "KILLING "  << execList[index]->instances << " INSTANCES" << endl;
	   for (int i = 0; i < execList[index]->instances; ++i) {
			 //			 cout << "KILLING instance "  << i  << endl;
			 execList[index]->mgrIn->putFullBuffer((void*)NULL);
	   }
	   for (int i = 0; i < execList[index]->instances; ++i) {
			 //			 cout << "WAITING for instance "  << i  << endl;
			 execList[index]->runningThreads[i]->join();
	   }
	   execList[index]->runningThreads.clear();

	   return execList[index]->instances;
}

int pipeExec::killPipe()
{
	   int killCount = 0;

	   for ( int i = 0; i < execList.size(); ++i) {
			 //			 cout << "Killing thread " << i << endl;
			 killCount += killNode(i);
	   }

	   return killCount;
}

// Returns the location index of the function funcToSearch. They are unique unless cloned
int pipeExec::findFunction(PipeBase *funcToSearch) {
	   for (int i = 0; i < execList.size(); ++i )
			 if ( execList[i]->procFunc == funcToSearch )
				    return i;
	   return -1;
}

// Insert a function before position
void pipeExec::insertFunction(PipeBase *func, int position, int instances, bool splice) {}

// Deletes function funcToSearch or function at position
void pipeExec::deleteFunction(PipeBase *funcToSearch, int position) {
	   int index;

	   index = findFunction(funcToSearch);
	   if ( index != -1 ) deleteNode(index);

	   cout << "deleteFunction: index = " << index << endl;
}

// Switch funcIn for funcOut
void pipeExec::switchFunc(PipeBase *funcIn, PipeBase *funcOut, int position) {}

// Collapse a function with multiple instances
void pipeExec::collapseFunc(PipeBase *funcToCollapse) {}


// A switchFunction(funcIn, funcOut) will allow for the dynamic modification of the pipe.
// Can created by
// 	1) searching for funcOut (i.e. pointer) in the execList
// 	2) stoping the thread
// 	3) changing the entry in execList
// 	4) launching the thread
// the cirtical issue is to make sure no data is being proccessed by the thread, that will be controlled by the
// execElement method. A semaphore will have to be added to determine when is the correct moment for the swith
// to be made

/* Delete			 
// Push the buffers down the pipe
if ( ! localArgs->isTail )
while ( true ) {
localArgs->mgrIn->waitForFull();
localArgs->mgrOut->putFullBuffer(localArgs->mgrIn->getFullBuffer());
}
else
while ( true ) {
localArgs->mgrIn->waitForFull();
localArgs->mgrOut->putFreeBuffer(localArgs->mgrIn->getFullBuffer());
}
*/
