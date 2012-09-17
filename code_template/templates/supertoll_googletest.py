#! /usr/bin/python

import os

from string import Template

import code_template_helpers 


filename_help = "Output file name(s)"
template = """/*
 * $FILENAME
 * Date: $DATE
 * Author: $AUTHOR ($AUTHOREMAIL)
 */

#include <sstream>

#include <gtest/gtest.h>

using namespace std;

TEST( TestName , TestCase )
{
    EXPECT_EQ( true , true );
}

"""



class supertoll_googletest( code_template_helpers.APlugin ):


    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , required=True )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        if hasattr( args , "filename" ) :
            for filename in args.filename:
                filename = code_template_helpers.check_filename_ending( filename , "cpp" )
                code_template_helpers.default_processing( filename , replacements , template )
