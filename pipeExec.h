// pipeExec

#ifndef _pipeExec_
#define _pipeExec_

#include <vector>
#include <thread>
#include "SimpleMemoryManager.h"

#include <iostream>

using namespace std;

class PipeBase;		// Forward declaration

class pipeExec {

	   public:

			 // HEAD position: HEAD->In = mgr, HEAD->out = mgr
			 pipeExec(PipeBase *func, SimpleMemoryManager* mgrIn, int instances = 1);

			 ~pipeExec();

			 // TAIL Last function in pipe: TAIL->In = PREV-Out, Tail->out = HEAD->In
			 // If splice is true, return to head and create a new 'head'
			 void addFunction(PipeBase *func, int instances = 1, bool splice = false);

			 // Insert a function before position
			 void insertFunction(PipeBase *func, int position, int instances = 1, bool splice = false);

			 // Returns the location index of the function funcToSearch. They are unique unless cloned
			 int findFunction(PipeBase *funcToSearch);

			 // Sends NULL data to the pipe to terminate the thread
			 int killNode(int index);

			 // Leaves a push data only node
			 void deleteNode(int index);

			 // Deletes function funcToSearch or function at position
			 void deleteFunction(PipeBase *funcToSearch = NULL, int position = 0);

			 // Switch funcIn for funcOut
			 void switchFunc(PipeBase *funcIn, PipeBase *funcOut = NULL, int position = 0);

			 // Collapse a function with multiple instances
			 void collapseFunc(PipeBase *funcToCollapse);

			 int runPipe();
			 int killPipe();


			 typedef struct  {
				    PipeBase		*procFunc;
				    SimpleMemoryManager*	currentHead; // Needed to implemnt splice
				    SimpleMemoryManager*	mgrIn;
				    SimpleMemoryManager*	mgrOut;
				    int			instances;
				    int			currentInstance;
				    bool			isTail; // Needed to implement splice
				    int			threadId;
				    bool			switching;
				    bool deleted;
				    std::mutex	stop;
				    std::vector< std::thread* >	runningThreads;
			 } pipeExecArgs;

	   private:

			 int count;
			 std::vector< pipeExecArgs* >	execList;
};

class PipeBase {
	   public:
			 virtual bool init() { return true; }
			 virtual bool run(void* args) { return true; } // Should return a void * so it can change data structure ?
			 virtual void end() { return; }

			 virtual PipeBase * clone() const = 0;
};

#endif
