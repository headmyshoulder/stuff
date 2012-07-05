#! /usr/bin/python

import os
import imp
from string import Template

from yapsy.IPlugin import IPlugin

def create_subparser( plugin , subparsers ):
    parser = subparsers.add_parser( plugin.name , help = plugin.help )
    parser.set_defaults( which = plugin.name )
    return parser

def add_filename_replacements( rep , filename ):
    fn = os.path.basename( filename )
    rep[ "FILENAMECAP" ] = ( os.path.splitext( fn )[0] ).upper()
    rep[ "FILEENDINGCAP" ] =  (( os.path.splitext( fn )[1] ).upper())[ 1: ]
    rep[ "FILENAME" ] = fn
    rep[ "DIRECTORY" ] = os.path.dirname( filename )



class simpleppp( IPlugin ):

    def __init__( self ):
        self.name = "simplecpp"
        self.help = "Creates a simple \"Hello world\" C++ file with one main function."
        self.filename_help = "Output file name(s)"
        self.template = """/*
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

    def set_base_path( self , path ):
        # helpers = imp.load_source( "code_template_helpers" , path )
        # print helpers
        # helpers.create_subparser( self , avc )

        f = None
        filename = None
        description = None
        try:
            f , filename , description = imp.find_module( "code_template_helpers" , [ path ] )
        except :
            print "Main module not found"
        prinf filename
        print description
        try:
            imp.load_module( "Helpers" , f , filename , description )
        finally:
            f.close()


    def register_in_arg_parser( self , subparsers ):
        parser = create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = self.filename_help )

    def do_work( self , args , default_replacements ):
        print "Creating " + self.name + " template(s) ..."
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                self.process( filename , default_replacements )

    def process( self , filename , default_replacements ):
        print "* Creating " + filename + " ..."
        replacements = default_replacements
        add_filename_replacements( replacements , filename )
        source = Template( self.template )
        f = open( filename , "w" );
        f.write( source.substitute( replacements ) )
