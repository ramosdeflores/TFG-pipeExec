class keyData {

	void setKeyData(string key, void* data) { keyData_.key = key; keyData_.data = data; }
	void* getData(return keyData_.data; }
	void getKey(return keyData_.key; }
	bool compKey(string key) { if ( keyData_.key == key ) return true; else return false: }

	private:

	struct {
		string key;
		void *data;
	} keyData_;
}

class dataObj {

	   ~dataObj() { delete(data); extraData.clear(); }

	   // Data is always preserved so it can be returned by the las element
	   void setData((void *)data) { data_ = data; }
	   void *getData() {  return data_; }

	   void setExtKeyData(string key, void *data) { keyData newData; newData.setKeyData(key, data); extraData_.push_back(newData); }
	   void setExtData(((void*)data) { setExtKeyData("_###_DEFAULT_KEY_###_", data); }
	   void *getExData((unsigned int index = 0) { if ( index < extraData.size() )  return extraData_.at(index)->data; else return NULL; }
	   void *findData(string key) { for (int i = 0; i < extraData_.size() ) if ( extraData_.at(i)->compKey(key) ) return extraData_.at(i)->data; return NULL; }
	   void deleteData(unsigned index = 0) { if ( index == -1 ) extraData_.clear(); else if ( index < extraData_.size() ) extraData.erase(index); else throw; }
	   void deleteKeyData(string key) { for (int i = 0; i < extraData_.size() ) if ( extraData_.at(i)->compKey(key) ) deleteData(i); }

	   void setIndex(unsigned int index) { index_ = index; }
	   unsigned int getIndex() { return index_; }

	   private:

	   unsigned int index_;
	   void *data_;
	   std::vector<keyData*> extraData_;
};

class compElement {

	   void interrupt() { mgrOut->setFullBuffer((void *)NULL); }

	   enum command { NONE, DELETE, INSERT, EXPAND, COLLAPSE, SWITCH };
	   enum status { INIT, DELETED, RUNNING, IDLE };
	   void setCommand(command cmd) { currentCommand = cmd; }
	   command getCommand() { return currentCommand; }

	   void setCurrentHead(SimpleStorageManager* storage) { currentHead = storage; }
	   SimpleStorageManager* getCurrentHead() { return currentHead; }
	   void setMgrIn(SimpleStorageManager* storage) { mgrIn = storage; }
	   SimpleStorageManager* getMgrIn() { return mgrIn; }
	   void setMgrOut(SimpleStorageManager* storage) { mgrOut = storage; }
	   SimpleStorageManager* getMgrIn() { return mgrIn; }
	   void setFunc(PipeBase *func) { procFunc = func; }
	   PipeBase* getFunc(PipeBase *func) { return procFunc; }
	   bool compFunc(PipeBase *func) { if ( procFunc == func ) return true; else return false; }

	   objData* getData() { mgrIn->waitForFull(); return mgrIn->getFullBuffer(); }
	   void putData(objData* data) { mgrOut->putFullBuffer(); }

	   status setStatus(status newStatus) { status hldStatus = currentStatus; currentStatus = newStatus; return hldStatus; }
	   status getStatus() { return currentStatus; }

	   void setThreadId(int id) { threadId = id; }
	   int getThreadId() { return threadId; }

	   void setRunningThread(std::thread* thread_) { runningThreads.push_back(thread_); }
	   std::thread* getRunningThread(unsigned int index = 0) { return runningThreads.at(index); } 
	   void deleteThread(unsigned index = 0) { if ( index == -1 ) runningThread.clear(); else if ( index < runningThread.size() ) runningThread.erase(index); else throw; }

	   void waitForThreads() { for(int i = 0; i < instances; ++i) runningThreads[i].join(); deleteThread(-1); };


	   std::mutex	stop;
	   bool	isTail;

	   private :
	   command currentCommand;
	   unsigned int count_;
	   SimpleStorageManager* currentHead;
	   SimpleStorageManager* mgrIn;
	   SimpleStorageManager* mgrOut;
	   PipeBase	*procFunc;
	   unsigned int	instances;
	   unsigned int	currentInstance;
	   status currentStatus;
	   int threadId;
	   std::vector< std::thread* >	runningThreads;
};
