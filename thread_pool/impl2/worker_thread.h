/*
 * worker_thread.h
 * Date: 2013-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef WORKER_THREAD_H_INCLUDED
#define WORKER_THREAD_H_INCLUDED


#include <memory>
#include <thread>


class threadpool_core;

class worker_thread
{
public:

    worker_thread( std::shared_ptr< threadpool_core > pool );

    void run( void );

    static std::shared_ptr< std::thread > create_thread( std::shared_ptr< threadpool_core > pool );

private:

    std::shared_ptr< threadpool_core > m_pool;
};



#endif // WORKER_THREAD_H_INCLUDED
