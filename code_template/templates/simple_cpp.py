#! /usr/bin/python

import os
import imp
import sys
from string import Template

import code_template_helpers 



filename_help = "Output file name(s)"
template = """/*
 * $FILENAME
 * Date: $DATE
 * Author: $AUTHOR ($AUTHOREMAIL)
 */

#include <iostream>

#define tab "\\t"

using namespace std;


int main( int argc , char *argv[] )
{
    cout << "Hello world!" << endl;
    return 0;
}
"""




class simpleppp( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = filename_help )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                filename = code_template_helpers.check_filename_ending( filename , "cpp" )
                code_template_helpers.default_processing( filename , replacements , template )
