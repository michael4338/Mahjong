#include "StdAfx.h"
#include "MJPoolExecutor.h"

#include <ace/Thread_Manager.h>
#include <exception>
#include <stdexcept>

MJPoolExecutor::MJPoolExecutor(int threadsNum, int queueSize) throw():
    m_nThreadsNum(threadsNum),
    m_nQueueSize(queueSize),
    m_bDeleteTaskFlag(false),
    m_bDestroyed(false)
{
}
MJPoolExecutor::~MJPoolExecutor() throw()
{
}

    
bool MJPoolExecutor::execute( ACE_Method_Request *task) throw() 
{
    if (task == NULL)
    {
        return false;
    }

    try 
    {
        int rc = m_queue.enqueue(task);
        if (rc >= 0)
        {
            return true;
        }
    }
    catch (std::runtime_error &ex)
    {
        return false;
    }

    return false;
}

void* MJPoolExecutor::svc_run(void* args) throw()
{
    MJPoolExecutor *thisObj = static_cast<MJPoolExecutor *> (args);
    if (thisObj == 0)
    {
        //AfxMessageBox("Null task queued.");
        return NULL;
    }
    thisObj->dispatch();
    return 0;
}

void MJPoolExecutor::start() throw() 
{
    m_nThreadGroupId = 
        ACE_Thread_Manager::instance()->spawn_n(m_nThreadsNum, 
        ACE_THR_FUNC(svc_run), this, THR_JOINABLE|THR_NEW_LWP|THR_INHERIT_SCHED);
}


// Change state.  No more new requests can be queued
// May have to add code to check if the queue is empty
void MJPoolExecutor::shutdown() throw()
{
    abort();
}

void MJPoolExecutor::abort() throw()
{
    // Drain the queue
    while(!m_queue.is_empty() )
    {
        ACE_Method_Request *method = m_queue.dequeue();
        if (method)
            delete method;
    }
    m_bDestroyed = true;
}

void MJPoolExecutor::dispatch() throw()
{
    while(true)
    {
        if ( m_bDestroyed ) 
            break;
            
        try 
        {
            ACE_Method_Request* method = m_queue.dequeue();
            if ( method == 0 ) 
                continue;
                
            method->call();
            
            if ( m_bDeleteTaskFlag ) 
                delete method;
        }
        catch (std::exception &ex) 
        {
        }
    }
}