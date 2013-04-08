/*
 * ThreadPool.h
 * Date: 2013-04-05
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <vector>
#include <tinythread.h>

class ThreadPool
{
public:

    ThreadPool();
    ~ThreadPool();

    void CreateThreads(int numOfThreads);

    // Assigns a job to a thread in the pool, but doesn't start the job
    // Each SubmitJob call will use up one thread of the pool.
    // This operation can only be undone by calling StartJobs and
    // then waiting for the jobs to complete. On completion,
    // new jobs may be submitted.
    void SubmitJob( void (*workFunc)(void *), void *workData );

    // Begins execution of all the jobs in the pool.
    void StartJobs();

    // Waits until all jobs have completed.
    // The wait will block the caller.
    // On completion, new jobs may be submitted.
    void WaitForJobsToComplete();

private:

    enum typeOfWorkEnum { e_work, e_quit };

    class ThreadData
    {
    public:

        bool ready;  // thread has been created and is ready for work  
        bool haveWorkToDo;
        typeOfWorkEnum  typeOfWork;

        // Pointer to the work function each thread has to call.
        void (*workFunc)(void *);

        // Pointer to work data
        void *workData;

        ThreadData() : ready(false), haveWorkToDo(false) {  };
    };

    struct ThreadArgStruct
    {
        ThreadPool *threadPoolInstance;
        int         threadId;
    };

    // Data for each thread
    ThreadData  *m_ThreadData;

    ThreadPool(ThreadPool const&); // copy ctor hidden
    ThreadPool& operator=(ThreadPool const&); // assign op. hidden

    // Static function that provides the function pointer that a thread can call
    // By including the ThreadPool instance in the void * parameter,
    // we can use it to access other data and methods in the ThreadPool instance.
    static void ThreadFuncWrapper(void *arg)
    {
        ThreadArgStruct *threadArg = static_cast<ThreadArgStruct *>(arg);
        threadArg->threadPoolInstance->ThreadFunc(threadArg->threadId);
    }

    // The function each thread calls    
    void ThreadFunc( int threadId );

    // Called by the thread pool destructor
    void DestroyThreadPool();

    // Total number of threads available
    // (fixed on creation of thread pool)
    int m_numOfThreads;
    int m_NumOfThreadsDoingWork;
    int m_NumOfThreadsGivenJobs;

    // List of threads
    std::vector<tthread::thread *> m_ThreadList;

    // Condition variable to signal each thread has been created and executing
    tthread::mutex              m_ThreadReady_mutex;
    tthread::condition_variable m_ThreadReady_condvar;

    // Condition variable to signal each thread to start work
    tthread::mutex              m_WorkToDo_mutex;
    tthread::condition_variable m_WorkToDo_condvar;

    // Condition variable to signal the main thread that 
    // all threads in the pool have completed their work
    tthread::mutex              m_WorkCompleted_mutex;
    tthread::condition_variable m_WorkCompleted_condvar;
};

#endif // THREADPOOL_H_INCLUDED
