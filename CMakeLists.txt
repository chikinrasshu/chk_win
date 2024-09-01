cmake_minimum_required(VERSION 3.27)
project(chk_win)

# Third party dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/LinkGLFW.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/LinkGLAD.cmake)

# Build the library
set(chk_win_src "chk_win.c")
set(chk_win_hdr "chk_win.h")

list(TRANSFORM chk_win_src PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")
list(TRANSFORM chk_win_hdr PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_library(chk_win ${chk_win_src} ${chk_win_hdr})
target_compile_features(chk_win PUBLIC c_std_11)
target_include_directories(chk_win PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

link_glfw(chk_win PRIVATE origin/master)
link_glad(chk_win PUBLIC origin/glad2)
