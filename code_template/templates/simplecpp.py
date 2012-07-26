#! /usr/bin/python

import os
import imp
import sys
from string import Template

from yapsy.IPlugin import IPlugin

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




class simpleppp( IPlugin ):

    def __init__( self ):
        self.name = "simplecpp"
        self.help = "Creates a simple \"Hello world\" C++ file with one main function."


    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = filename_help )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                code_template_helpers.default_processing( filename , replacements , template )