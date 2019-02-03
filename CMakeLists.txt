cmake_minimum_required(VERSION 3.0)
project(status C)

include(GNUInstallDirs)

find_package(ALSA REQUIRED)

add_executable(statusbar "statusbar.c")

target_sources(statusbar PRIVATE
              "${CMAKE_CURRENT_SOURCE_DIR}/update_volume.c"
              "${CMAKE_CURRENT_SOURCE_DIR}/update_mpd.c"
              "${CMAKE_CURRENT_SOURCE_DIR}/update_time.c"
              )

target_link_libraries(statusbar PRIVATE mpdclient X11 ${ALSA_LIBRARIES})
target_compile_options(statusbar PRIVATE -Wall -pedantic-errors -std=c11)
target_compile_definitions(statusbar PRIVATE _POSIX_C_SOURCE)

install(TARGETS statusbar
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        CONFIGURATIONS Release)
