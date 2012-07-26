/*
 * rule_engine.h
 * Date: 2012-07-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */


#ifndef RULE_ENGINE_H_INCLUDED
#define RULE_ENGINE_H_INCLUDED

#include <utility>

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/type_erasure/constructible.hpp>
#include <boost/type_erasure/relaxed_match.hpp>




namespace rule_engine
{

    namespace te = boost::type_erasure;
    namespace mpl = boost::mpl;


    template< class Return , class Context >
    struct requirements : public mpl::vector<
        te::copy_constructible<> ,
        te::typeid_<> ,
        te::callable< Return( Context ) > ,
        te::relaxed_match 
        > { };

    // specialization for void
    template< class Return >
    struct requirements< Return , void > : public mpl::vector<
        te::copy_constructible<> ,
        te::typeid_<> ,
        te::callable< Return( void ) > ,
        te::relaxed_match 
        > { };

    template< class Return , class Context >
    using expr = te::any< requirements< Return , Context > > ;


    template< class Context >
    using boolean_expr = expr< bool , Context >;


    template< class Return , class Context >
    class unary_expr
    {
    public:
        typedef expr< Return , Context > expr_type;
        
        template< class T >
        unary_expr( T child ) : m_child( std::move( child ) ) { }

        unary_expr( expr_type child ) : m_child( child ) { }

        expr_type& child( void ) { return m_child; }
        const expr_type& child( void ) const { return m_child; }
    protected:
        expr_type m_child;
    };

    template< class Return , class Context >
    class binary_expr
    {
    public:
        typedef expr< Return , Context > expr_type;

        template< class T1 , class T2 >
        binary_expr( T1 left , T2 right ) : m_left( std::move( left ) ) , m_right( std::move( right ) ) { }

        template< class T1 >
        binary_expr( T1 left , expr_type right ) : m_left( std::move( left ) ) , m_right( right ) { }

        template< class T2 >
        binary_expr( expr_type left , T2 right ) : m_left( left ) , m_right( std::move( right ) ) { }

        binary_expr( expr_type left , expr_type right ) : m_left( left ) , m_right( right ) { }

        expr_type& left( void ) { return m_left; }
        const expr_type& left( void ) const { return m_left; }
        expr_type& right( void ) { return m_right; }
        const expr_type& right( void ) const { return m_right; }

    protected:

        expr_type m_left , m_right;
    };



    struct true_
    {
        template< class Context >
        bool operator()( Context ) const { return true; }
        bool operator()( void ) const { return true; }
    };

    struct false_
    {
        template< class Context >
        bool operator()( Context ) const { return false; }
        bool operator()( void ) const { return false; }
    };

    template< class Context >
    class not_ : public unary_expr< bool , Context >
    {
    public:

        typedef boolean_expr< Context > expr_type;

        template< class T >
        not_( T child ) : unary_expr< bool , Context >( child ) { }
        bool operator()( Context c ) { return ! this->m_child( c ); }
    };

    template<>
    class not_< void > : public unary_expr< bool , void >
    {
    public:

        typedef boolean_expr< void > expr_type;

        template< class T >
        not_( T child ) : unary_expr< bool , void >( child ) { }
        bool operator()( void ) { return ! this->m_child(); }
    };

    template< class Context >
    class and_ : public binary_expr< bool , Context >
    {
    public:

        typedef boolean_expr< Context > expr_type;

        template< class T1 , class T2 >
        and_( T1 left , T2 right ) : binary_expr< bool , Context >( left , right ) { } 
        bool operator()( Context c ) { return this->m_left( c ) && this->m_right( c ); }
    };

    template<>
    class and_< void > : public binary_expr< bool , void  >
    {
    public:

        typedef boolean_expr< void > expr_type;

        template< class T1 , class T2 >
        and_( T1 left , T2 right ) : binary_expr< bool , void >( left , right ) { } 
        bool operator()( void ) { return this->m_left( ) && this->m_right( ); }
    };

    template< class Context >
    class or_ : public binary_expr< bool , Context >
    {
    public:

        typedef boolean_expr< Context > expr_type;

        template< class T1 , class T2 >
        or_( T1 left , T2 right ) : binary_expr< bool , Context >( left , right ) { } 
        bool operator()( Context c ) { return this->m_left( c ) || this->m_right( c ); }
    };

    template<>
    class or_< void > : public binary_expr< bool , void >
    {
    public:

        typedef boolean_expr< void > expr_type;

        template< class T1 , class T2 >
        or_( T1 left , T2 right ) : binary_expr< bool , void >( left , right ) { } 
        bool operator()( void ) { return this->m_left() || this->m_right(); }
    };


}



#endif // RULE_ENGINE_H_INCLUDED
