# Date: 2013-02-20
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( EO )
cmake_minimum_required ( VERSION 2.8 )

# Add path for custom modules
set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

set ( EO_ROOT_DIR "/home/karsten/boost/testing/ml/EOOLD" )

find_library ( EO NAMES eo PATHS ${EO_ROOT_DIR}/build/lib )
find_library ( EOUTILS NAMES eoutils PATHS ${EO_ROOT_DIR}/build/lib )

find_package ( Boost REQUIRED )

if ( CMAKE_COMPILER_IS_GNUCXX )
    set ( CMAKE_CXX_FLAGS "-std=c++0x" )        ## Optimize
endif ()

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${EO_ROOT_DIR}/src )

add_executable ( main main.cpp )

target_link_libraries( main ${EO} ${EOUTILS} )
