cmake_minimum_required(VERSION 3.15)
project(BA C)

set(CMAKE_C_STANDARD 99)

add_library(sharedlib SHARED sharedlib.c sharedlib.h)

add_executable(poc poc.c lib.c lib.h)
target_link_libraries(poc pthread)

add_executable(threshholds threshholds.c lib.c lib.h)
target_link_libraries(threshholds pthread)

add_executable(poc_sync poc_sync.c lib.c lib.h)
target_link_libraries(poc_sync pthread)

add_executable(frame_poc frame_poc.c lib.c lib.h)
target_link_libraries(frame_poc pthread sharedlib)

add_executable(sender sender.c lib.c lib.h)
target_link_libraries(sender pthread sharedlib)

add_executable(receiver receiver.c lib.c lib.h)
target_link_libraries(receiver pthread sharedlib)