/*
 * threadpool_core.h
 * Date: 2013-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef THREADPOOL_CORE_H_INCLUDED
#define THREADPOOL_CORE_H_INCLUDED

#include <memory>
#include <functional>
#include <vector>
#include <deque>
#include <thread>

class worker_thread;

class threadpool_core : public std::enable_shared_from_this< threadpool_core >
{
public:

    typedef std::function< void( void ) > task_type;

    threadpool_core( size_t num_of_threads = 2 );
    ~threadpool_core( void );

    void add_task( const task_type &t );
    bool execute_task( void );

    void create_threads( void );
    void destroy( void );
    void wait( void );

    size_t m_num_of_threads;
    std::deque< task_type > m_tasks;
    std::vector< std::shared_ptr< std::thread > > m_threads;
};


#endif // THREADPOOL_CORE_H_INCLUDED
