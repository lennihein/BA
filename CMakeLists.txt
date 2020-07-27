project(BA C)

set(CMAKE_C_STANDARD 99)

add_library(sharedlib SHARED sharedlib.c sharedlib.h)

add_executable(meassure_send meassure_send.c lib.c lib.h)
target_link_libraries(meassure_send pthread sharedlib)

add_executable(meassure_recv meassure_recv.c lib.c lib.h)
target_link_libraries(meassure_recv pthread sharedlib)

add_executable(sender sender.c lib.c lib.h crc.c)
target_link_libraries(sender pthread sharedlib)

add_executable(receiver receiver.c lib.c lib.h crc.c)
target_link_libraries(receiver pthread sharedlib)

add_executable(send_ascii send_ascii.c lib.c lib.h crc.c)
target_link_libraries(send_ascii pthread sharedlib)

add_executable(receive_ascii receive_ascii.c lib.c lib.h crc.c)
target_link_libraries(receive_ascii pthread sharedlib)