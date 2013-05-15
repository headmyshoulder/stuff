/*
 * threadsafe_queue.h
 * Date: 2013-04-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef THREADSAFE_QUEUE_H_INCLUDED
#define THREADSAFE_QUEUE_H_INCLUDED

#include <queue> 
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
public:

    threadsafe_queue( void )
    {}

    void push( T new_value )
    {
        std::lock_guard< std::mutex > lk( m_mut );
        m_data_queue.push( std::move( new_value ) );
        data_cond.notify_one();
    }

    void wait_and_pop( T& value)
    {
        std::unique_lock< std::mutex > lk( m_mut );
        data_cond.wait( lk , [this] { return !m_data_queue.empty(); } );
        value = std::move( m_data_queue.front() );
        m_data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop( void )
    {
        std::unique_lock< std::mutex > lk( m_mut );
        data_cond.wait( lk , [this]{ return !m_data_queue.empty(); } );
        std::shared_ptr<T> res( std::make_shared<T>( std::move( m_data_queue.front() ) ) );
        m_data_queue.pop();
        return res;
    }

    bool try_pop( T& value )
    {
        std::lock_guard< std::mutex > lk( m_mut );
        if( m_data_queue.empty() )
            return false;
        value = std::move( m_data_queue.front() );
        m_data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop( void )
    {
        std::lock_guard< std::mutex > lk( m_mut );
        if( m_data_queue.empty() )
            return std::shared_ptr<T>();
        std::shared_ptr<T> res( std::make_shared<T>( std::move( m_data_queue.front() ) ) );
        m_data_queue.pop();
        return res;
    }

    bool empty( void ) const
    {
        std::lock_guard< std::mutex > lk( m_mut );
        return m_data_queue.empty();
    }

    size_t size( void ) const
    {
        std::lock_guard< std::mutex > lk( m_mut );
        return m_data_queue.size();
    }

private:

    mutable std::mutex m_mut;
    std::queue<T> m_data_queue;
    std::condition_variable data_cond;

};



#endif // THREADSAFE_QUEUE_H_INCLUDED
