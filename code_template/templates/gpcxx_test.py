#! /usr/bin/python

import os
import sys
from string import Template

import code_template_helpers 



filename_help = "Output file name(s)"
test_help = "Name of the test."
template = """/*
 $BOOSTFILENAME

 Copyright 2013 Karsten Ahnert

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <gtest/gtest.h>

#include <sstream>

#define TESTNAME $TESTNAME

using namespace std;

TEST( TESTNAME , TestCase )
{
    EXPECT_EQ( true , true );
}

"""


        



class gpcxx_test( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = filename_help )
        parser.add_argument( "-t" , "--test" , nargs = 1 , help = test_help )



    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        path = code_template_helpers.find_boost_path( [ "test" ] )

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
