#! /usr/bin/python

import os

from string import Template

import code_template_helpers 


filename_help = "Output file name(s)"
target_help = "Cpp targets to be created."
template = """# Date: $DATE
# Author: $AUTHOR ($AUTHOREMAIL)

project (PROJECT)
cmake_minimum_required (VERSION 2.6) 

# Add path for custom modules
set(CMAKE_MODULE_PATH
  $${CMAKE_MODULE_PATH}
  $${CMAKE_CURRENT_SOURCE_DIR}/cmake
  $${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package (Boost REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++0x")        ## Optimize
endif()

include_directories ( $${Boost_INCLUDE_DIRS} )

$TARGETS
"""



class simplemakefile( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , default=["CMakeLists.txt"] )
        parser.add_argument( "-t" , "--target" , nargs = "*" , help = target_help )



    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        replacements[ "TARGETS" ] = ""
        if ( hasattr( args , "target" ) ) and ( args.target is not None ) and ( len( args.target ) != 0 ) :
            for target in args.target:
                print "* Found target " + target
                p = os.path.splitext( target )
                if p[1] == ".cpp" :
                    replacements[ "TARGETS" ] += "add_executable ( " + p[0] + " " + p[0] + ".cpp" + " ) \n"
                else:
                    replacements[ "TARGETS" ] += "add_executable ( " + target + " " + target + ".cpp" + " ) \n"

            
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                code_template_helpers.default_processing( filename , replacements , template )
