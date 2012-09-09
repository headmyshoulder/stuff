#! /usr/bin/python

import os

from string import Template

import code_template_helpers 


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



class supertoll_header( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , required=True )
        parser.add_argument( "-n" , "--namespace" , nargs = "*" , help = namespace_help )
        parser.add_argument( "-c" , "--class" , nargs = "*" , help = class_help , dest = "classes" )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        code_template_helpers.add_namespace_replacements( replacements , args , [ "SuperToll" ] )
        code_template_helpers.add_class_replacements( replacements , args , code_template_helpers.default_class_template )
            
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                filename = code_template_helpers.check_filename_ending( filename , "h" )
                code_template_helpers.default_processing( filename , replacements , template )
