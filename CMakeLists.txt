cmake_minimum_required(VERSION 3.15)
project(BA C)

set(CMAKE_C_STANDARD 99)

add_library(sharedlib SHARED sharedlib.c sharedlib.h)

add_executable(threshholds threshholds.c lib.c lib.h)
target_link_libraries(threshholds pthread)

add_executable(sender sender.c lib.c lib.h)
target_link_libraries(sender pthread sharedlib)

add_executable(receiver receiver.c lib.c lib.h)
target_link_libraries(receiver pthread sharedlib)