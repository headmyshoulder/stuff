#! /usr/bin/python

import os
import glob

searchdirs = [ os.path.join( os.path.dirname(__file__) , "templates" ) ]


files = []
for dir in searchdirs:
    for f in glob.glob( os.path.join( dir , "*.py" ) ):
        files.append( f )
__all__ = files

