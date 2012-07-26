/*
 * rule_engine.h
 * Date: 2012-07-26
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <iostream>

#include "rule_engine.h"

#define tab "\t"

using namespace std;
using namespace rule_engine;

void test1( void )
{
    cout << "starting test1 with context type \"void\" " << endl;
    using expr_type = boolean_expr< void >;
    using not_ = not_< void >;
    using and_ = and_< void >;
    using or_ = or_< void >;

    true_ t;
    false_ f;

    expr_type e1( t );
    cout << e1() << endl;

    not_ n( t );
    expr_type e2( n );
    cout << e2() << endl;

    // expr_type e3( not_( f ) );
    // cout << e3() << endl;

    expr_type e4( and_( t , or_( f , t ) ) );
    cout << e4() << endl;
    cout << "Finished! " << endl << endl;

    expr_type e5;
    cout << e5() << endl;
}





struct context
{
    int value_a( void ) const { return 10; }
    int value_b( void ) const { return 10; }
};

struct context_a_less
{
    int m_cmp;
    context_a_less( int cmp ) : m_cmp( cmp ) { };
    bool operator()( const context &s ) { return s.value_a() < m_cmp; }
};

struct context_b_less
{
    int m_cmp;
    context_b_less( int cmp ) : m_cmp( cmp ) { };
    bool operator()( const context &s ) { return s.value_b() < m_cmp; }
};



void test2( void )
{
    cout << "starting test2 with context type \"context\"" << endl;
    using expr_type = boolean_expr< const context& >;
    using not_ = not_< const context& >;
    using and_ = and_< const context& >;
    using or_ = or_< const context& >;

    context s;

    true_ t;
    false_ f;

    expr_type e1( t );
    cout << e1( s ) << endl;

    expr_type e2( context_b_less( 5 ) );
    cout << e2( s ) << endl;

    expr_type e3( context_b_less( 15 ) );
    cout << e3( s ) << endl;

    expr_type e4( and_( context_a_less( 11 ) , context_b_less( 15 ) ) );
    cout << e4( s ) << endl;

    cout << "Finished! " << endl << endl;
}


int main( int argc , char *argv[] )
{
    test1();
    test2();

    return 0;
}
