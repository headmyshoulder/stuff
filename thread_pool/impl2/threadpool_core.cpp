/*
 * threadpool_core.cpp
 * Date: 2013-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "threadpool_core.h"
#include "worker_thread.h"



#include <iostream>
#define tab "\t"
using namespace std;


threadpool_core::threadpool_core( size_t num_of_threads )
    : m_num_of_threads( num_of_threads )
{
}
    
threadpool_core::~threadpool_core( void )
{
    wait();
    destroy();
}

void threadpool_core::add_task( const task_type &t )
{
    m_tasks.push_back( t );
}

bool threadpool_core::execute_task( void )
{
    cout << "Run task from thread " << std::this_thread::get_id() << endl;
    while( m_tasks.empty() )
    {
        cout << "Waiting for jobs" << endl;
    }
    cout << "Found job" << endl;

    task_type task = m_tasks.front();
    m_tasks.pop_front();
    task();

    return true;
}

void threadpool_core::create_threads( void )
{
    for( size_t i=0 ; i<m_num_of_threads ; ++i )
    {
        m_threads.push_back( worker_thread::create_thread( shared_from_this() ) );
    }
}

void threadpool_core::destroy( void )
{
    for( size_t i=0 ; i<m_threads.size() ; ++i )
        m_threads[i]->join();
}

void threadpool_core::wait( void )
{
}
