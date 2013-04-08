# Date: 2013-04-04
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

add_executable ( main_impl2 main.cpp worker_thread.cpp threadpool_core.cpp )
target_link_libraries ( main_impl2 ${Boost_LIBRARIES} pthread )
