/*
 * Copyright (c) 2011 Adrian Michel
 * http://www.amichel.com
 *
 * Permission to use, copy, modify, distribute and sell this 
 * software and its documentation for any purpose is hereby 
 * granted without fee, provided that both the above copyright 
 * notice and this permission notice appear in all copies and in 
 * the supporting documentation. 
 *  
 * This library is distributed in the hope that it will be 
 * useful. However, Adrian Michel makes no representations about
 * the suitability of this software for any purpose.  It is 
 * provided "as is" without any express or implied warranty. 
 * 
 * Should you find this library useful, please email 
 * info@amichel.com with a link or other reference 
 * to your work. 
*/

#include <differential_evolution.hpp>
#include "objective_function.h"

using namespace de;

/** 
 * Objective function to optimize is the "sphere function": 
 *  
 * f(x,y) = x^2 + y^2 
*/
class sphere_function
{
public:
	sphere_function()
//	: objective_function( "sphere function" )
	{
	}

	virtual double operator()( de::DVectorPtr args )
	{
		/**
		 * The two function arguments are the elements index 0 and 1 in 
		 * the argument vector, as defined by the constraints vector 
		 * below 
		 */
		double x = (*args)[ 0 ];
		double y = (*args)[ 1 ];

		return x*x + y*y;
	}
};

#define VARS_COUNT 20
#define POPULATION_SIZE 200

void simpleUsage()
{
	try
	{
		/**
		 * Create and initialize the constraints object 
		 *  
		 * First create it with default constraints (double type, min 
		 * -1.0e6, max 1.0e6) then set the first two elements to be of 
		 *  type real with x between -10, 10 and y between -100, 100.
		 */
		constraints_ptr constraints( boost::make_shared< constraints >( VARS_COUNT , -1.0e6, 1.0e6 ) );
		(*constraints)[ 0 ] = boost::make_shared< real_constraint >( -10, 10 );
		(*constraints)[ 1 ] = boost::make_shared< real_constraint >( -100, 100 );

		/**
		 * Instantiate the objective function 
		 *  
		 * The objective function can be any function or functor that 
		 * takes a de::DVectorPtr as argument and returns a double. It 
		 * can be passed as a reference, pointer or shared pointer. 
		 */
		sphere_function of;

		/**
		 * Instantiate two null listeners, one for the differential 
		 * evolution, the other one for the processors 
		 */
		listener_ptr listener( boost::make_shared< null_listener >() );
		processor_listener_ptr processor_listener( boost::make_shared< null_processor_listener >() );

		/**
		 * Instantiate the collection of processors with the number of 
		 * parallel processors (4), the objective function and the 
		 * listener 
		 */
		processors< sphere_function >::processors_ptr _processors( boost::make_shared< processors< sphere_function > >( 4, boost::ref( of ), processor_listener ) );

		/**
		 * Instantiate a simple termination strategy wich will stop the 
		 * optimization process after 10000 generations 
		 */
		termination_strategy_ptr terminationStrategy( boost::make_shared< max_gen_termination_strategy >( 10000 ) );

		/**
		 * Instantiate the selection strategy - we'll use the best of 
		 * parent/child strategy 
		 */
		selection_strategy_ptr selectionStrategy( boost::make_shared< best_parent_child_selection_strategy >() );

		/**
		 * Instantiate the mutation strategy - we'll use the mutation 
		 * strategy 1 with the weight and crossover factors set to 0.5 
		 * and 0.9 respectively 
		 */
		mutation_strategy_arguments mutation_arguments( 0.5, 0.9 );
		mutation_strategy_ptr mutationStrategy( boost::make_shared< mutation_strategy_1 >( VARS_COUNT, mutation_arguments ) );

		/**
		 * Instantiate the differential evolution using the previously 
		 * defined constraints, processors, listener, and the various 
		 * strategies 
		 */
		differential_evolution< sphere_function > de( VARS_COUNT, POPULATION_SIZE, _processors, constraints, true, terminationStrategy, selectionStrategy, mutationStrategy, listener );

		/**
		 * Run the optimization process
		 */
		de.run();

		/**
		 * Get the best individual resulted from the optimization 
		 * process 
		 */
		individual_ptr best( de.best() );

		/**
		 * Print out the result
		 */
		std::cout << "minimum value for the " << /*of->name() << */" is " << best->cost() << " for x=" << (*best->vars())[ 0 ] << ", y=" << (*best->vars())[ 1 ];
	}
	catch( const de::exception& e )
	{
		/**
		 * Print out any errors that happened during the initialization 
		 * or optimization phases
		 */
		std::cout << "an error occurred: " << e.what();
	}
}
