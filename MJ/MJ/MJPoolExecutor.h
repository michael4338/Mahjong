#pragma once

#include "ace/Activation_Queue.h"
#include "ace/Method_Request.h"
#include <ace/Synch.h>

class MJPoolExecutor
{
public:
    MJPoolExecutor(int threadsNum, int queueSize) throw();
    virtual ~MJPoolExecutor() throw();

    //##Documentation
    //## If you enqueue tasks when the queue is full, execute will block.
    int getQueueSize() const throw() {return m_nQueueSize;}

    //##Documentation
    //## This method returns the number of worker threads associated with this
    //## pool.  If threads should die, the pool will generate the threads.
    int getNumberOfThreads()  const throw() {return m_nThreadsNum;}
    bool getDeleteTaskFlag() const throw() {return m_bDeleteTaskFlag;}
    void setDeleteTaskFlag(bool flag) throw() {m_bDeleteTaskFlag = flag;}

    //##Documentation
    //## This method is assigned to a thread group.  This method in turn can
    //## process requests.
    static void *svc_run(void *args) throw();
       
    //##Documentation
    //## This method deposits the task on a queue and returns.  This will block
    //## if the queue is full.
    bool execute( ACE_Method_Request *task) throw();


    void start() throw();
    void shutdown() throw();
    void abort() throw();
  
private:
    ACE_Activation_Queue m_queue;
    int m_nThreadsNum;
    int m_nThreadGroupId;
    int m_nQueueSize;
    bool m_bDeleteTaskFlag;
    bool m_bDestroyed;
     
    //##Documentation
    //## svc_run, the static method calls dispatch.  Dispatch dequeues the next
    //## request and executes it.
    void dispatch() throw();
};
