/*
 * locking_ptr.h
 * Date: 2013-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef LOCKING_PTR_H_INCLUDED
#define LOCKING_PTR_H_INCLUDED

#include <boost/noncopyable.hpp>


template <typename T, typename Mutex>
class locking_ptr : private boost::noncopyable
{
    T* m_obj;
    Mutex & m_mutex;

public:
    /// Constructor.
    locking_ptr( volatile T& obj , const volatile Mutex& mtx )
        : m_obj( const_cast< T* >( &obj ) )
        , m_mutex( *const_cast< Mutex* >( &mtx ) )
    {   
        m_mutex.lock();
    }

    ~locking_ptr( void )
    { 
        m_mutex.unlock();
    }


    T& operator*( void ) const
    {    
        return *m_obj;    
    }

    T* operator->( void ) const
    {   
        return m_obj;   
    }
};


#endif // LOCKING_PTR_H_INCLUDED
