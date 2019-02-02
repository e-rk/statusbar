cmake_minimum_required(VERSION 3.0)
project(status)

include(GNUInstallDirs)

add_executable(statusbar "statusbar.c")
target_link_libraries(statusbar PUBLIC mpdclient X11)

install(TARGETS statusbar 
        RUNTIME DESTINATION bin
        CONFIGURATIONS Release)
