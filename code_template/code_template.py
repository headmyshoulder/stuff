#! /usr/bin/python

import argparse
import datetime
import os
import glob

from yapsy.PluginManager import PluginManager


description = "Create source code templates for commonly used files."

directories = [ os.path.join( os.path.dirname( os.path.realpath(__file__) ) , "templates" ) ]

config = {
    "Author" : "Karsten Ahnert" ,
    "AuthorEmail" : "karsten.ahnert@gmx.de"
}

default_replacements = {
    "AUTHOR" : config[ "Author" ] ,
    "AUTHOREMAIL" : config[ "AuthorEmail" ] ,
    "DATE" : datetime.date.today().isoformat()
}


parser = argparse.ArgumentParser( description = description )
subparsers = parser.add_subparsers( help="subcommand help" )
templates = {}



def register_plugin( plugin_info ):
    plugin = plugin_info.plugin_object
    plugin.register_in_arg_parser( subparsers )
    templates[ plugin.name ] = plugin

def main():   

    # Load and register all plugins
    manager = PluginManager( directories_list = directories )
    manager.collectPlugins()
    for plugin in manager.getAllPlugins():
        register_plugin( plugin ) 

    # parse arguments and evaluate the current template
    args = parser.parse_args()
    template = templates[ args.which ]
    template.do_work( args , default_replacements )

    

if __name__ == "__main__" :

    main()
    





# def get_template_files( directories ) :
#     files = []
#     for dir in directories :
#         fns = glob.glob( os.path.join( dir , "*.json" ) )
#         for f in fns :
#             files.append( f )
#     return files



# def add_template_to_argparser( data , subparser ):
#     parser = subparsers.add_parser(
#         data[ "name" ] ,
#         help = data[ "help" ] )
#     for p in data[ "parameters" ]:
#         parser.add_argument( p[ "filename" ] ,  nargs = 1 , help = p[ "help" ] )
#     parser.set_defaults( which = data[ "name" ])

# def get_replacements( template , args ):
#     rep = default_replacements
#     if hasattr( args , "filename" ) :
#         fn = os.path.basename( args.filename[0] )
#         rep[ "FILENAMECAP" ] = ( os.path.splitext( fn )[0] ).upper()
#         ext = ( os.path.splitext( fn )[1] ).upper()
#         rep[ "FILEENDINGCAP" ] = ext[ 1: ]
#         rep[ "FILENAME" ] = fn
#     return rep



# def create_template( template , args ):
#     reps = get_replacements( template , args )
#     cont = open( os.path.join( template[ "dirname" ] , template[ "code" ] ) , "r" ).read()
#     abc = Template( cont )
#     if hasattr( args , "filename" ):
#         f = open( args.filename[0] , "w"  );
#         f.write( abc.substitute( reps ) )






# files = get_template_files( directories )

# ( parser , subparsers ) = create_arg_parser()

# templates = {}

# for f in files:
#     data = json.load( open( f , "r" ) )
#     data[ "dirname" ] = os.path.dirname( f )
#     add_template_to_argparser( data , subparsers )
#     templates[ data[ "name" ] ] = data



# args = parser.parse_args()

# template = templates[ args.which ]

# create_template( template , args )
