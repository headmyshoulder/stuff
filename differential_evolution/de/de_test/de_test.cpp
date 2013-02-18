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
#include <listener.hpp>

#include "testfunctions.h"
#include "cmdline.h"

using namespace de;

/**
 * Basic Differential Evolution listener - displays the 
 * generation cout and the best cost. 
 * 
 * @author adrian (12/8/2011)
 */
class DETestListener : public de::listener
{
public:
	virtual void start()
	{
	}

	virtual void end()
	{
	}

	virtual void error()
	{
	}

	virtual void startGeneration( size_t genCount )
	{
	}

	virtual void endGeneration( size_t genCount, individual_ptr bestIndGen, individual_ptr bestInd )
	{
		std::cout << ( boost::format( "genCount: %1%, cost: %2%\n" ) % genCount % bestInd->cost() ).str();
	}

	virtual void startSelection( size_t genCount )
	{
	}

	virtual void endSelection( size_t genCount )
	{
	}

	virtual void startProcessors( size_t genCount )
	{
	}

	virtual void endProcessors( size_t genCount )
	{
	}
};

/**
 * Very basic processor listener that doesn't do anything. 
 *  
 * It shows however how to setup a thread safe listener, by 
 * using synchronization objects. 
 * 
 * @author adrian (12/8/2011)
 */
class DETestProcessorListener : public processor_listener
{
	de::mutex m_mx;
public:
	virtual void start( size_t index )
	{
		de::lock lock( m_mx );
	}

	virtual void start_of( size_t index, individual_ptr ind )
	{
		de::lock lock( m_mx );
	}

	virtual void end_of( size_t index, individual_ptr ind )
	{
		de::lock lock( m_mx );
	}

	virtual void end( size_t index )
	{
		de::lock lock( m_mx );
	}

	virtual void error( size_t index, const std::string& message )
	{
		de::lock lock( m_mx );
	}

};

/**
 * Runs the Differential Evolution optimization process on 
 * function and with the parameters selected on the command line
 * 
 * @author adrian (12/8/2011)
 * 
 * @param cmdLine 
 */
void testFunctions( const CmdLine& cmdLine )
{
	// get the constraints as defined on the command line
	constraints_ptr constraints( cmdLine.constraints() );
	assert( constraints );

	// get the objective function as selected on the command line
	objective_function_ptr of( cmdLine.functionToOptimize() );
	assert( of );

	// instantiate a DE listener
	de::listener_ptr listener( boost::make_shared< DETestListener >() );

	// instantiate a Processors listener
	processor_listener_ptr processorListener( boost::make_shared< DETestProcessorListener >() );

	// instantiate the Processors, using the number of processors defined on the command line, and the processors listener
	processors< objective_function_ptr >::processors_ptr processors( boost::make_shared< processors< objective_function_ptr > >( cmdLine.processorsCount(), of, processorListener ) );

	// instantiate a basic termination strategy (just count the # of generations)
	termination_strategy_ptr terminationStrategy( boost::make_shared< max_gen_termination_strategy >( cmdLine.maxGenerations() ) );

	// instantiate the selection and mutation strategies as selected on the command line
	selection_strategy_ptr selectionStrategy( cmdLine.selectionStrategy() );
	mutation_strategy_ptr mutationStrategy( cmdLine.mutationStrategy() );

	// show a message with some basic facts about the session
	std::cout << ( cmdLine.minimize() ? "minimizing \"" : "maximizing \"" ) << of->name() << "\" with weight factor " << cmdLine.weight() << " and crossover factor " << cmdLine.crossover() << std::endl << std::endl;;

	// create a differential_evolution object using all the parameters defined above or on the command line
	differential_evolution< objective_function_ptr > de( cmdLine.argumentsCount(), cmdLine.populationSize(), processors, constraints, cmdLine.minimize(), terminationStrategy, selectionStrategy, mutationStrategy, listener );

	// run the optization process
	de.run();
}

int main(int argc, char* argv[])
{
	try
	{
		// instantiate a command ine object
		CmdLine cmdLine;

		// if command line processing was succesful, run the test
		if( cmdLine.process( argc, argv ) )
			testFunctions( cmdLine );

		return 0;
	}
	catch( const CmdLineException& e )
	{
		// there has been a cmd line error
		std::cout << "Command line parameter error: " << e.what() << std::endl;
		return 1;
	}
	catch( const de::exception& e )
	{
		// there has been some error, must likely triggered by a boost object.
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch( ... )
	{
		// catching all other exceptions, so the process won't crash
		// this most likely indicates a bug (memory issue, null pointer etc).
		std::cout << "Unknown exception" << std::endl;
	}
}
