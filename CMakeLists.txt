cmake_minimum_required(VERSION 3.0)
project(status C)

include(GNUInstallDirs)

add_executable(statusbar "statusbar.c")

target_link_libraries(statusbar PRIVATE mpdclient X11) 
target_compile_options(statusbar PRIVATE -Wall -pedantic-errors -std=c11)

install(TARGETS statusbar 
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        CONFIGURATIONS Release)
