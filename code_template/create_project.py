#! /usr/bin/python


import sys
import os

if len( sys.argv ) < 2:
    print "usage project_name!"
    exit( -1 )

prj = sys.argv[1]


os.mkdir( prj )
os.chdir( prj )

os.system( "code_template.py SimpleCMakeLists" )
os.system( "code_template.py SimpleCpp main" )
os.mkdir( "build" )

