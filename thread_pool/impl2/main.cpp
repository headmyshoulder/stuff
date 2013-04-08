/*
 * cond.cpp
 * Date: 2013-04-06
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "threadpool.h"

#include <Amboss/Util/Timer.h>

#include <iostream>
#include <random>

#define tab "\t"

using namespace std;
using namespace Amboss::Util;

void task( size_t seed , size_t number )
{
    std::mt19937 rng( seed );
    std::normal_distribution<> dist( 0.0 , 1.0 );
    double sum = 0.0;
    for( size_t i=0 ; i<number ; ++i )
        sum += dist( rng );
    sum /= double( number );
    cerr << sum << endl;
}


int main( int argc , char *argv[] )
{
    Timer t;
    {
        size_t number = 10000;
        threadpool tp( 2 );
        
        std::mt19937 rng;

        // Add tasks
        for( size_t i=0 ; i<1000 ; ++i )
            tp.add_task( std::bind( task , rng() , number ) );

        // while( !tp.empty() )
        // {
        //     cout << "Currently " << tp.pending() << " jobs are waiting for execution. Elapsed time is "
        //          << t.seconds() << " seconds!" << endl;
        //     boost::this_thread::sleep_for( boost::chrono::milliseconds( 154 ) );
        // }
    }
    cout << t.seconds() << endl;

    
    
    return 0;
}
