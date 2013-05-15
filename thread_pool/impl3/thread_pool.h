/*
 * thread_pool.h
 * Date: 2013-04-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef THREAD_POOL_H_INCLUDED
#define THREAD_POOL_H_INCLUDED

#include "threadsafe_queue.h"

#include <vector>
#include <thread>
#include <future>


class join_threads
{
public:

    typedef std::vector< std::thread > thread_container;

    explicit join_threads( thread_container& threads )
        : m_threads( threads )
    { }
    
    ~join_threads()
    {
        for( size_t i=0 ; i<m_threads.size() ; ++i )
        {
            if( m_threads[i].joinable() ) m_threads[i].join();
        }
    }

private:

    thread_container& m_threads;
};

class function_wrapper
{
public:

    template<typename F>
    function_wrapper(F&& f)
        : impl( new impl_type< F >( std::move( f ) ) )
    {}

    void operator()( void ) { impl->call(); }

    function_wrapper() = default;
    function_wrapper( function_wrapper&& other )
        : impl(std::move(other.impl))
    {}

    function_wrapper& operator=(function_wrapper&& other)
    {
        impl = std::move( other.impl );
        return *this;
    }

    function_wrapper(const function_wrapper&)=delete;
    function_wrapper(function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&)=delete;

private:

    struct impl_base {
        virtual void call( void ) = 0;
        virtual ~impl_base( void ) {}
    };

    template<typename F>
    struct impl_type : impl_base
    {
        F f;
        impl_type( F&& f_ ) : f( std::move( f_ ) ) {}
        void call( void ) { f(); }
    };

    std::unique_ptr< impl_base > impl;
};


class thread_pool
{
public:

    typedef function_wrapper task_type;
    // typedef std::function< void( void ) > task_type;

    thread_pool( size_t thread_count )
        : m_done( false ) , m_joiner( m_threads )
    {
        try
        {
            for( size_t i=0 ; i<thread_count ; ++i )
            {
                m_threads.push_back( std::thread( &thread_pool::worker_thread , this ) );

                std::unique_lock< std::mutex > lk( m_mutex );
                m_start_condition.wait( lk );
            }
        }
        catch( ... )
        {
            m_done = true;
            throw;
        }
    }
    
    ~thread_pool()
    {
        m_done = true;
    }

    // template< typename FunctionType >
    // void submit( FunctionType f )
    // {
    //     m_work_queue.push( f );
    // }

    template< typename FunctionType >
    std::future< typename std::result_of< FunctionType() >::type >
    submit( FunctionType f )
    {
        typedef typename std::result_of< FunctionType() >::type result_type;

        std::packaged_task< result_type() > task( std::move( f ) );
        std::future< result_type > res( task.get_future() );
        m_work_queue.push( std::move( task ) );
        return res;
    }

    bool empty( void ) const { return m_work_queue.empty(); }

    size_t pending( void ) const { return m_work_queue.size(); }


private:

    void worker_thread()
    {
        m_start_condition.notify_one();
        while( !m_done )
        {
            task_type task;
            if( m_work_queue.try_pop( task ) )
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }


    std::atomic_bool m_done;
    threadsafe_queue< task_type > m_work_queue;
    std::vector< std::thread > m_threads;
    join_threads m_joiner;

    std::mutex m_mutex;
    std::condition_variable m_start_condition;

};


#endif // THREAD_POOL_H_INCLUDED
