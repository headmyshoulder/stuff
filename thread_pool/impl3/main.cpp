/*
 * main2.cpp
 * Date: 2013-04-13
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "thread_pool.h"

#include <Amboss/Util/Timer.h>

#include <iostream>
#include <random>
#include <fstream>

#define tab "\t"

using std::cout;
using std::cerr;
using std::endl;
using Amboss::Util::Timer;

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


int main( int argc , char *argv[] )
{
    // thread_pool pool( 4 );
    // std::future< double > res = pool.submit( task2( 0 , 2000 ) );
    // cout << res.get() << endl;

    std::ofstream fout( "result.dat" );
    

    size_t number = 2000000;
    size_t num_of_tasks = 100;

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
        Timer t;
        thread_pool tp( num_of_threads );
        std::mt19937 rng;

        std::vector< std::future< double > > results;

        for( size_t i=0 ; i<num_of_tasks ; ++i )
        {
            results.push_back( tp.submit( task2( rng() , number ) ) );
        }

        while( !tp.empty() )
        {
            // cout << "Currently " << tp.pending() << " jobs are waiting for execution. Elapsed time is "
            //      << t.seconds() << " seconds!" << endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
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
