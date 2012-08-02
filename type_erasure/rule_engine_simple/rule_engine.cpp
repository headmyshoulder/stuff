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



int main( int argc , char *argv[] )
{
    true_ t;
    false_ f;
    expr e( t );
    cout << e() << endl;

    not_ n( t );
    cout << n() << endl;

    not_ n2( e );
    cout << n2() << endl;


    expr e2( and_( or_( f , t ) , not_( f ) ) );
    cout << e2() << endl;

    expr e3( n );
    cout << e3() << endl;

    and_ a( or_( f , t ) , not_( f ) );
    a.left() = e3;

    expr e4( a );
    cout << e4() << endl;


    expr e5( std::move( not_( f ) ) );    // why does that not ?
    cout << e5() << endl;

    expr e6( e );
    cout << e6() << endl;


    return 0;
}
