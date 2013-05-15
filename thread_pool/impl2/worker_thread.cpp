/*
 * worker_thread.cpp
 * Date: 2013-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "worker_thread.h"
#include "threadpool_core.h"

#include <iostream>
#define tab "\t"
using namespace std;

worker_thread::worker_thread( std::shared_ptr< threadpool_core > pool )
    : m_pool( pool )
{
}

void worker_thread::run( void )
{
    cout << "Starting run from thread " << std::this_thread::get_id() << endl;
    while( m_pool->execute_task() ) { cout << "Next step!" << endl; }
    cout << "Finishing run from thread " << std::this_thread::get_id() << endl;
}


std::shared_ptr< std::thread > worker_thread::create_thread( std::shared_ptr< threadpool_core > pool )
{
    std::shared_ptr< worker_thread > worker = std::make_shared< worker_thread >( pool );
    std::shared_ptr< std::thread > t = std::make_shared< std::thread >( &worker_thread::run , worker );
    cout << "Created worker thread with id " << t->get_id() << endl;
    return t;
}
