/*
 * ThreadPool.cpp
 * Date: 2013-04-05
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */


#include "ThreadPool.h"

#include <iostream>


// This is the thread function for each thread.
// All threads remain in this function until
// they are asked to quit, which only happens
// when terminating the thread pool.
void ThreadPool::ThreadFunc( int threadId )
{
    ThreadData *myThreadData = &m_ThreadData[threadId]; 
    std::cout << "Hello world: Thread " << threadId << std::endl;

    // Signal that this thread is ready
    m_ThreadReady_mutex.lock();
    myThreadData->ready = true;
    m_ThreadReady_condvar.notify_one(); // notify the main thread
    m_ThreadReady_mutex.unlock();       

    while(true)
    {
        //tthread::lock_guard<tthread::mutex> guard(m);
        m_WorkToDo_mutex.lock();

        while(!myThreadData->haveWorkToDo) // check for work to do
            m_WorkToDo_condvar.wait(m_WorkToDo_mutex); // if no work, wait here 
        myThreadData->haveWorkToDo = false; // need to do this before unlocking the mutex

        m_WorkToDo_mutex.unlock();

        // Do the work
        switch(myThreadData->typeOfWork)
        {
        case e_work:
            std::cout << "Thread " << threadId << ": Woken with work to do\n";

            // Do work
            myThreadData->workFunc(myThreadData->workData);

            std::cout << "#Thread " << threadId  << ": Work is completed\n";
            break;

        case e_quit:
            std::cout << "Thread " << threadId << ": Asked to quit\n";
            return; // ends the thread
        }

        // Now to signal the main thread that my work is completed
        m_WorkCompleted_mutex.lock();
        m_NumOfThreadsDoingWork--;

        // Unsure if this 'if' would make the program more efficient
        // if(m_NumOfThreadsDoingWork == 0)
        m_WorkCompleted_condvar.notify_one(); // notify the main thread
        m_WorkCompleted_mutex.unlock();       
    }

}


ThreadPool::ThreadPool() 
{ 
    m_numOfThreads = 0;  m_NumOfThreadsDoingWork = 0; m_NumOfThreadsGivenJobs = 0;
}


ThreadPool::~ThreadPool()
{
    if(m_numOfThreads)
    {
        DestroyThreadPool(); 
        delete [] m_ThreadData;
    }
}


void ThreadPool::CreateThreads(int numOfThreads)
{
// Check if a thread pool has already been created
    if(m_numOfThreads > 0) 
        return;

    m_NumOfThreadsGivenJobs = 0;
    m_NumOfThreadsDoingWork = 0;
    m_numOfThreads = numOfThreads;
    m_ThreadData = new ThreadData[m_numOfThreads];
    ThreadArgStruct threadArg;

    for(int i=0; i<m_numOfThreads; ++i)
    {   
        threadArg.threadId = i;
        threadArg.threadPoolInstance = this;

        // Creates the thread and saves it in a list so we can destroy it later
        m_ThreadList.push_back( new tthread::thread( ThreadFuncWrapper, (void *)&threadArg  ) ); 

        // It takes a little time for a thread to get established.
        // Best wait until it gets established before creating the next thread.
        m_ThreadReady_mutex.lock();
        while(!m_ThreadData[i].ready)  // Check if thread is ready
            m_ThreadReady_condvar.wait(m_ThreadReady_mutex); // If not, wait here
        m_ThreadReady_mutex.unlock();    
    } 
} 


// Assigns a job to a thread, but doesn't start the job
void ThreadPool::SubmitJob(void (*workFunc)(void *), void *workData)
{
    // Check if the thread pool has been created
    if(!m_numOfThreads) 
        return;

    if(m_NumOfThreadsGivenJobs >= m_numOfThreads)
        return;

    m_ThreadData[m_NumOfThreadsGivenJobs].workFunc = workFunc;
    m_ThreadData[m_NumOfThreadsGivenJobs].workData = workData;  

    std::cout << "Submitted job " << m_NumOfThreadsGivenJobs << std::endl;

    m_NumOfThreadsGivenJobs++;  
}

void ThreadPool::StartJobs()
{
// Check that the thread pool has been created
// and some jobs have been assigned
    if(!m_numOfThreads || !m_NumOfThreadsGivenJobs) 
        return;

// Set 'haveworkToDo' flag for all threads 
    m_WorkToDo_mutex.lock();
    for(int i=0; i<m_NumOfThreadsGivenJobs; ++i)
    {
        m_ThreadData[i].typeOfWork = e_work;  // forgot to do this !
        m_ThreadData[i].haveWorkToDo = true;
    }
    m_NumOfThreadsDoingWork = m_NumOfThreadsGivenJobs;

    // Reset this counter so we can resubmit jobs later
    m_NumOfThreadsGivenJobs = 0;

    // Notify all threads they have work to do
    m_WorkToDo_condvar.notify_all();
    m_WorkToDo_mutex.unlock();
}


void ThreadPool::WaitForJobsToComplete()
{
    // Check that a thread pool has been created
    if(!m_numOfThreads) 
        return;

    m_WorkCompleted_mutex.lock();
    while(m_NumOfThreadsDoingWork > 0)  // Check if all threads have completed their work
        m_WorkCompleted_condvar.wait(m_WorkCompleted_mutex); // If not, wait here
    m_WorkCompleted_mutex.unlock();    
}


void ThreadPool::DestroyThreadPool()
{
    std::cout << "Ask threads to quit\n";
    m_WorkToDo_mutex.lock();
    for(int i=0; i<m_numOfThreads; ++i)
    {
        m_ThreadData[i].haveWorkToDo = true;
        m_ThreadData[i].typeOfWork = e_quit;
    }
    m_WorkToDo_condvar.notify_all();
    m_WorkToDo_mutex.unlock();

// As each thread terminates, catch them here
    for(int i=0; i<m_numOfThreads; ++i)
    {
        tthread::thread *t = m_ThreadList[i];

        // Wait for thread to complete
        t->join();
    }
    m_numOfThreads = 0;
}
