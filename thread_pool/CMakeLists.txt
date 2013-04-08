# Date: 2013-04-04
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( PROJECT )
cmake_minimum_required ( VERSION 2.6 )

# Add path for custom modules
set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package ( Boost REQUIRED COMPONENTS system thread )
find_package ( Amboss REQUIRED )

set ( THREAD_POOL_INCLUDE_DIR "/home/karsten/boost/testing/threadpool-0_2_5-src/threadpool" )

if ( CMAKE_COMPILER_IS_GNUCXX )
    set ( CMAKE_CXX_FLAGS "-std=c++0x" )        ## Optimize
endif ()



include_directories ( ${Amboss_INCLUDE_DIR} )
include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${THREAD_POOL_INCLUDE_DIR} )

add_subdirectory ( impl1 )
add_subdirectory ( impl2 )


add_executable ( main main.cpp )
target_link_libraries ( main ${Boost_LIBRARIES} )

