#! /usr/bin/python

import os

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
