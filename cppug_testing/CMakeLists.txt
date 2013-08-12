# Date: 2013-08-12
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( PROJECT )
cmake_minimum_required ( VERSION 2.6 )

# Add path for custom modules
set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package ( Boost REQUIRED )

if ( CMAKE_COMPILER_IS_GNUCXX )
    set ( CMAKE_CXX_FLAGS "-std=c++11" )        ## Optimize
endif ()

include_directories ( ${Boost_INCLUDE_DIRS} )


add_executable ( main main.cpp )