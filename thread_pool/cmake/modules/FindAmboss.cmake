#
# http://root.cern.ch/viewvc/trunk/cint/reflex/cmake/modules/FindCppUnit.cmake
#
# - Find CppUnit
# This module finds an installed CppUnit package.
#
# It sets the following variables:
#  Amboss_FOUND       - Set to false, or undefined, if CppUnit isn't found.
#  Amboss_INCLUDE_DIR - The Amboss include directory.



set ( Amboss_ROOT $ENV{AMBOSS_ROOT} )

find_path ( Amboss_INCLUDE_DIR Amboss/KML.h ${Amboss_ROOT}/include )

if ( Amboss_INCLUDE_DIR )
   set ( Amboss_FOUND TRUE )
else ()
  set ( Amboss_FOUND FALSE )
endif ()


if ( Amboss_FOUND )

   # show which Amboss Directory was found only if not quiet
   if ( NOT Amboss_FIND_QUIETLY )
      message ( STATUS "Found Amboss include directory : ${Amboss_INCLUDE_DIR}" )
   endif ( NOT Amboss_FIND_QUIETLY )

else ()

   # fatal error if Amboss is required but not found
   if ( Amboss_FIND_REQUIRED )
      message ( FATAL_ERROR "Could not find Amboss. Make sure Amboss can be found in the default include path or in the environment variable AMBOSS_ROOT" )
   else ()
     message ( STATUS "Could not find Amboss. Make sure Amboss can be found in the default include path or in the environment variable AMBOSS_ROOT" )
   endif ()

endif ()
