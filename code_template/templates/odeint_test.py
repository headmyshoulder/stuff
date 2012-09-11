#! /usr/bin/python

import os
import sys
from string import Template

import code_template_helpers 



filename_help = "Output file name(s)"
test_help = "Name of the test."
template = """/*
 [auto_generated]
 $BOOSTFILENAME

 [begin_description]
 tba.
 [end_description]

 Copyright 2009-2012 Karsten Ahnert
 Copyright 2009-2012 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/config.hpp>
#ifdef BOOST_MSVC
    #pragma warning(disable:4996)
#endif

#define BOOST_TEST_MODULE odeint_$TESTNAME

#include <boost/test/unit_test.hpp>

#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>

using namespace boost::unit_test;
using namespace boost::numeric::odeint;


BOOST_AUTO_TEST_SUITE( ${TESTNAME}_test )

BOOST_AUTO_TEST_CASE( test_case1 )
{
    BOOST_CHECK_EQUAL( 1 , 1 );
}

BOOST_AUTO_TEST_SUITE_END()
"""


        



class odeint_test( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = filename_help )
        parser.add_argument( "-t" , "--test" , nargs = 1 , help = test_help )



    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        path = code_template_helpers.find_odeint_lib_path()

        replacements[ "TESTNAME" ] = "dummy"
        if hasattr( args , "test" ) and ( args.test is not None ) and ( len( args.test ) == 1 ):
            print "Found test " + args.test[0]
            replacements[ "TESTNAME" ] = args.test[0]


        if hasattr( args , "filename" ) :
            for filename in args.filename:
                filename = code_template_helpers.check_filename_ending( filename , "cpp" )
                p = path
                p.append( filename )
                replacements[ "BOOSTFILENAME" ] = code_template_helpers.full_join( p )
                code_template_helpers.default_processing( filename , replacements , template )
