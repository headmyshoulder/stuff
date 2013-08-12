#! /usr/bin/python

import os

from string import Template

import code_template_helpers 


filename_help = "Output file name(s)"
namespace_help = "Additional namespace definitions to be created. gpcxx will be always created."
class_help = "Class template(s) to be created" 
template = """/*
  $BOOSTFILENAME

  Copyright 2013 Karsten Ahnert

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or
  copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef $BOOSTFILENAME_CAP
#define $BOOSTFILENAME_CAP


$NAMESPACE_OPENING

$CLASS_TEMPLATE

$NAMESPACE_CLOSING

#endif // $BOOSTFILENAME_CAP
"""



class gpcxx_header( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , required=True )
        parser.add_argument( "-n" , "--namespace" , nargs = "*" , help = namespace_help )
        parser.add_argument( "-c" , "--class" , nargs = "*" , help = class_help , dest = "classes" )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        path = code_template_helpers.find_gpcxx_path()

        code_template_helpers.add_namespace_replacements( replacements , args , [ "gpcxx" ] )
        code_template_helpers.add_class_replacements( replacements , args , code_template_helpers.default_class_template )
            
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                filename = code_template_helpers.check_filename_ending( filename , "hpp" )
                p = path
                p.append( filename )
                f = code_template_helpers.full_join( p )
                replacements[ "BOOSTFILENAME" ] = f
                replacements[ "BOOSTFILENAME_CAP" ] = code_template_helpers.create_cap_boost_filename_str( f )
                code_template_helpers.default_processing( filename , replacements , template )
