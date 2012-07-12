#! /usr/bin/python

import os

from string import Template
from yapsy.IPlugin import IPlugin

from code_template_helpers import *


filename_help = "Output file name(s)"
namespace_help = "Additional namespace definitions to be created."
class_help = "Class templates to be created" 
template = """/*
 * $FILENAME
 * Date: $DATE
 * Author: $AUTHOR ($AUTHOREMAIL)
 */

#ifndef SUPERTOLL_${FILENAMECAP}_${FILEENDINGCAP}_INCLUDED
#define SUPERTOLL_${FILENAMECAP}_${FILEENDINGCAP}_INCLUDED


$NAMESPACE_OPENING

$CLASS_TEMPLATE

$NAMESPACE_CLOSING

#endif // SUPERTOLL_${FILENAMECAP}_${FILEENDINGCAP}_INCLUDED
"""



class supertoll_header( IPlugin ):

    def __init__( self ):
        self.name = "supertollheader"
        self.help = "Creates a header file with header guards and namespace defintions for SuperToll."


    def register_in_arg_parser( self , subparsers ):
        parser = create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , required=True )
        parser.add_argument( "-n" , "--namespace" , nargs = "*" , help = namespace_help )
        parser.add_argument( "-c" , "--class" , nargs = "*" , help = class_help , dest = "classes" )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        add_namespace_replacements( replacements , args , "SuperToll" )
        add_class_replacements( replacements , args , default_class_template )
            
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                default_processing( filename , replacements , template )
