#! /usr/bin/python

import os

from string import Template
from yapsy.IPlugin import IPlugin

import code_template_helpers


filename_help = "Output file name(s)"
template = """/*
 * $FILENAME
 * Date: $DATE
 * Author: $AUTHOR ($AUTHOREMAIL)
 * Copyright: $AUTHOR
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>

using namespace std;

int main( int argc , char **argv )
{
    cout << "Hello world!" << endl;
    return 0;
}
"""


class supertoll_source( IPlugin ):

    def __init__( self ):
        self.name = "ambossmain"
        self.help = "Creates a simple \"Hello world!\" application for Amboss."


    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = filename_help )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        if hasattr( args , "filename" ) :
            for filename in args.filename:
                code_template_helpers.default_processing( filename , replacements , template )
