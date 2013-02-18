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

/**
 * \defgroup de_test de_test command line application 
 * @{ 
 */

#ifndef DE_TEST_CMDLINE_H_INCLUDED
#define DE_TEST_CMDLINE_H_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <de_constraints.hpp>
#include <selection_strategy.hpp>
#include <mutation_strategy.hpp>

#include "objective_function.h"


/**
 * Encapsulation of the function to optimize
 * 
 * @author adrian (12/8/2011)
 */
class FunctionToOptimize
{
  static objective_function_ptr _functions[];
  size_t _index;
public:
  FunctionToOptimize( size_t index );

  objective_function_ptr operator()() const;
};

/**
 * Shared pointer to a FunctionToOptimize
 */
typedef boost::shared_ptr< FunctionToOptimize > FunctionToOptimizePtr;

/**
 * Command line class
 * 
 * @author adrian (12/8/2011)
 */
class CmdLine
{
private:                                                  
  std::string                _description;
  double                     _weight;
  double                     _crossover;
  size_t                     _populationSize;
  size_t                     _maxGenerations;
  de::mutation_strategy_ptr  _mutationStrategy;
  bool                       _minimize;
  FunctionToOptimizePtr      _functionToOptimize;
  de::selection_strategy_ptr _selectionStrategy;
  size_t                     _processorsCount;
  de::constraints_ptr        _constraints;
  size_t                     _argumentsCount;

public:
  CmdLine();
  bool process( int argc, char* argv[] );

  std::string getUsage() const;

  void showUsage();
  void notice( std::ostream& os );
  void showError( const std::string& error );

  size_t                     populationSize()     const { return  _populationSize;    }
  size_t                     maxGenerations()     const { return  _maxGenerations;    }
  de::mutation_strategy_ptr  mutationStrategy()   const { return  _mutationStrategy;  }
  size_t                     processorsCount()    const { return  _processorsCount;   }
  double                     weight()             const { return  _weight;            }
  double                     crossover()          const { return  _crossover;         }
  bool                       minimize()           const { return  _minimize;          }
  de::constraints_ptr        constraints()        const { return  _constraints;       }
  size_t                     argumentsCount()     const { return  _argumentsCount;    }
  objective_function_ptr functionToOptimize() const { assert( _functionToOptimize ); return (*_functionToOptimize)(); }
  de::selection_strategy_ptr selectionStrategy()  const { assert( _selectionStrategy  ); return _selectionStrategy;       }
};

/**
 * Command line exception
 * 
 * @author adrian (12/8/2011)
 */
class CmdLineException : public de::exception
{
public:
  CmdLineException( const std::string& message ) : de::exception( message.c_str() )
  {
  }
};

/**
 * @}
 */

#endif //DE_TEST_CMDLINE_H_INCLUDED
