#! /usr/bin/python

import os
import sys
from string import Template

import code_template_helpers 



filename_help = "Output file name(s)"
prefix_hellp = "Prefix for the filename, for example a directory boost/numeric/libs/example"
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

#include <iostream>

#include <boost/numeric/odeint.hpp>

namespace odeint = boost::numeric::odeint;

int main( int argc , char *argv[] )
{
    std::cout << "Hello world!" << std::endl;
    return 0;
}
"""


        



class simpleppp( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = filename_help )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        path = code_template_helpers.find_odeint_lib_path()


        if hasattr( args , "filename" ) :
            for filename in args.filename:
                filename = code_template_helpers.check_filename_ending( filename , "cpp" )
                p = path
                p.append( filename )
                replacements[ "BOOSTFILENAME" ] = code_template_helpers.full_join( p )
                code_template_helpers.default_processing( filename , replacements , template )
