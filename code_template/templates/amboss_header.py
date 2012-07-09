#! /usr/bin/python

import os

from string import Template
from yapsy.IPlugin import IPlugin

from code_template_helpers import *


filename_help = "Output file name(s)"
namespace_help = "Namespace definitions to be created."
class_help = "Class templates to be created" 
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


$NAMESPACE_OPENING

$CLASS_TEMPLATE

$NAMESPACE_CLOSING
"""



class supertoll_header( IPlugin ):

    def __init__( self ):
        self.name = "ambossheader"
        self.help = "Creates a simple header with header guards for Amboss."


    def register_in_arg_parser( self , subparsers ):
        parser = create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , required=True )
        parser.add_argument( "-n" , "--namespace" , nargs = "*" , help = namespace_help )
        parser.add_argument( "-c" , "--class" , nargs = "*" , help = class_help , dest = "classes" )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        add_namespace_replacements( replacements , args , "Amboss" )
        add_class_replacements( replacements , args , default_class_template )
      
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                default_processing( filename , replacements , template )


    


    
    
