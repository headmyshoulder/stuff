#include <differential_evolution.hpp>
#include "my_listener.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

using namespace de;

template< class T >
T min3( T t1 , T t2 , T t3 )
{
    using std::min;
    return min( t1 , min( t2 , t3 ) );
}

class fitness_function 
{
public:


    fitness_function( const std::vector< double > &x1 , const std::vector< double > &x2 , const std::vector< double > &y ) 
        : m_x1( x1 ) , m_x2( x2 ) , m_y( y )
    {
    }

    double operator()( de::DVectorPtr args )
    {
        double a1 = (*args)[ 0 ];
        double a2 = (*args)[ 1 ];
        double a3 = (*args)[ 2 ];
        double a4 = (*args)[ 2 ];
        double a5 = (*args)[ 2 ];

        double chi2 = 0.0;
        size_t N = min3( m_x1.size() , m_x2.size() , m_y.size() );
        for( size_t i=0 ; i<N ; ++i )
        {
            double x1 = m_x1[i] , x2 = m_x2[i];
            double val = a1 * ( x1*x1 + x2*x2 ) + a2 * cos( a3 * x1 + a4 ) + a5;
            chi2 += ( val - m_y[i] ) * ( val - m_y[i] );
        }
        return chi2 ;
    }

    std::string name() const { return "Fitness function"; }

private:

    const std::vector< double > &m_x1;
    const std::vector< double > &m_x2;
    const std::vector< double > &m_y;
};

template< class Rng >
void create_data( std::vector< double > &x1 , std::vector< double > &x2 , std::vector< double > &y , size_t N , Rng &rng ,
                  double a1 = 0.25 , double a2 = 2.0 , double a3 = 1.0 , double a4 = 0.5 , double a5 = -3.3 )
{
    std::normal_distribution<> dist1( 0.0 , 1.0 ) , dist2( 0.0 , 1.0 );
    x1.resize( N ); 
    x2.resize( N ); 
    y.resize( N ); 
    for( size_t i=0 ; i<N ; ++i )
    {
        double x1_ = dist1( rng ) , x2_ = dist2( rng );
        x1[i] = x1_;
        x2[i] = x2_;
        y[i] = a1 * ( x1_ * x1_ + x2_ * x2_ ) + a2 * cos( a3 * x1_ + a4 ) + a5;
    }
}

typedef fitness_function objective_function;
typedef boost::shared_ptr< objective_function > objective_function_ptr;


int main( int argc , char **argv )
{
    const size_t vars_count = 5;
    const size_t population_size = 2000;

    std::vector< double > x1 , x2 , y;
    std::mt19937 rng;
    create_data( x1 , x2 , y , 10000 , rng );

    constraints_ptr constraints( boost::make_shared< constraints >( vars_count , -1.0e6, 1.0e6 ) );
    (*constraints)[ 0 ] = boost::make_shared< real_constraint >( -10.0 , 10.0 );
    (*constraints)[ 1 ] = boost::make_shared< real_constraint >( -10 , 10 );
    (*constraints)[ 2 ] = boost::make_shared< real_constraint >( 0.0 , 10 );
    (*constraints)[ 3 ] = boost::make_shared< real_constraint >( 0.0 , M_PI );
    (*constraints)[ 4 ] = boost::make_shared< real_constraint >( -10 , 10 );

    objective_function_ptr of( boost::make_shared< objective_function >( x1 , x2 , y ) );

    listener_ptr listener( boost::make_shared< my_listener >( std::cout ) );
    processor_listener_ptr processor_listener( boost::make_shared< null_processor_listener >() );

    processors< objective_function_ptr >::processors_ptr _processors(
        boost::make_shared< processors< objective_function_ptr > >( 4, of, processor_listener ) );

    termination_strategy_ptr terminationStrategy( boost::make_shared< max_gen_termination_strategy >( 10000 ) );

    selection_strategy_ptr selectionStrategy( boost::make_shared< best_parent_child_selection_strategy >() );

    mutation_strategy_arguments mutation_arguments( 0.5, 0.9 );
    mutation_strategy_ptr mutationStrategy( boost::make_shared< mutation_strategy_3 >( vars_count, mutation_arguments ) );

    differential_evolution< objective_function_ptr > de( vars_count, population_size, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener );

    de.run();

    individual_ptr best( de.best() );

    std::cout << "minimum value for the " << of->name() << " is " << best->cost() << " for x=" << (*best->vars())[ 0 ] << ", y=" << (*best->vars())[ 1 ] << std::endl;
}
