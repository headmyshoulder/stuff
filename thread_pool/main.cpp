/*
 * main.cpp
 * Date: 2013-04-04
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#define TIME_UTC TIME_UTC_

#include <boost/threadpool.hpp>
#include <boost/thread.hpp>

#include <functional>
#include <iostream>
#include <random>
#include <fstream>

#include <Amboss/Util/Timer.h>

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

struct task2
{
    size_t m_seed;
    size_t m_number;

    typedef double result_type;

    template< typename T >
    struct result { typedef double type; };

    task2( size_t seed , size_t number ) : m_seed( seed ) , m_number( number ) { }

    double operator()( void ) const
    {
        std::mt19937 rng( m_seed );
        std::normal_distribution<> dist( 0.0 , 1.0 );
        double sum = 0.0;
        for( size_t i=0 ; i<m_number ; ++i )
            sum += dist( rng );
        sum /= double( m_number );
        return sum;
    }
};

double task3( void ) { return 1.0; }



int main( int argc , char *argv[] )
{
    // Timer t;
    // {
    //     size_t number = 10000;
    //     boost::threadpool::pool tp( 2 );

    //     std::mt19937 rng;

    //     // Add tasks
    //     for( size_t i=0 ; i<1000 ; ++i )
    //         tp.schedule( std::bind( task , rng() , number ) );

    //     while( !tp.empty() )
    //     {
    //         cout << "Currently " << tp.pending() << " jobs are waiting for execution. Elapsed time is "
    //              << t.seconds() << " seconds!" << endl;
    //         boost::this_thread::sleep_for( boost::chrono::milliseconds( 154 ) );
    //     }
    // }
    // cout << t.seconds() << endl;

    ofstream fout( "result.dat" );


    size_t number = 100000;
    size_t num_of_tasks = 1000;

    {
        Timer t;
        std::mt19937 rng;
        
        double sum = 0.0;
        for( size_t i=0 ; i<num_of_tasks ; ++i )
            sum += task2( rng() , number )();
        sum /= double( num_of_tasks );
        double sec = t.seconds();
        cout << "Result without pool is " << sum << " obtained in " << t.seconds() << " seconds!" << endl;
        fout << 1 << tab << sec << tab << sum << endl << endl;
    }


    for( size_t num_of_threads=1 ; num_of_threads<16 ; ++num_of_threads )
    {
//        size_t num_of_threads = 3;

        Timer t;
        boost::threadpool::pool tp( num_of_threads );
        std::mt19937 rng;

        std::vector< boost::threadpool::future< double > > results;

        for( size_t i=0 ; i<num_of_tasks ; ++i )
        {
            boost::function< double( void ) > func = task2( rng() , number );
            results.push_back( boost::threadpool::schedule( tp , func ) );
        }

        while( !tp.empty() )
        {
            // cout << "Currently " << tp.pending() << " jobs are waiting for execution. Elapsed time is "
            //      << t.seconds() << " seconds!" << endl;
            boost::this_thread::sleep_for( boost::chrono::milliseconds( 1 ) );
        }
        // cout << "Finished all jobs in " << t.seconds() << " seconds!" << endl;
        double sum = 0.0;
        for( size_t i=0 ; i<results.size() ; ++i ) sum += results[i].get();
        sum /= double( results.size() );
        double sec = t.seconds();
        cout << "Result for " << num_of_threads << " threads is " << sum << " obtained in " << sec << " seconds!" << endl;
        fout << num_of_threads << tab << sec << tab << sum << endl;
    }


    return 0;
}
