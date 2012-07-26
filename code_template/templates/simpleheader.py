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

}
#ifndef ${FILENAMECAP}_${FILEENDINGCAP}_INCLUDED
#define ${FILENAMECAP}_${FILEENDINGCAP}_INCLUDED



#endif // ${FILENAMECAP}_${FILEENDINGCAP}_INCLUDED
"""




class simpleppp( IPlugin ):

    def __init__( self ):
        self.name = "simpleheader"
        self.help = "Creates a simple C++ header file with header guards."


    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = filename_help )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                code_template_helpers.default_processing( filename , replacements , template )
