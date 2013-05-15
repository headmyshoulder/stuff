/*
 * threadpool.h
 * Date: 2013-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED


#include "threadpool_core.h"

#include <memory>


class threadpool
{
public:

    // using task_type = threadpool_core::task_type;
    typedef threadpool_core::task_type task_type;

    threadpool( size_t num_of_threads = 2 )
        : m_core( std::make_shared< threadpool_core >( num_of_threads ) )
    {
        m_core->create_threads();
    }
    
    ~threadpool( void )
    {
    }

    void wait( void )
    {
    }

    void add_task( const task_type &t )
    {
        m_core->add_task( t );
    }

private:

    std::shared_ptr< threadpool_core > m_core;
};



#endif // THREADPOOL_H_INCLUDED
