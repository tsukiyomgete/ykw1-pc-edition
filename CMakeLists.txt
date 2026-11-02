cmake_minimum_required(VERSION 3.20)
project(ykw1_pc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(ykw1 ${SOURCES})
target_include_directories(ykw1 PRIVATE src)

# SDL2: gamepad, keyboard, window and sound (installed with MSYS2 pacman)
find_package(SDL2 REQUIRED CONFIG)
target_compile_definitions(ykw1 PRIVATE SDL_MAIN_HANDLED)
if(TARGET SDL2::SDL2-static)
    target_link_libraries(ykw1 PRIVATE SDL2::SDL2-static)
else()
    target_link_libraries(ykw1 PRIVATE SDL2::SDL2)
endif()


# Put libstdc++, libgcc and winpthread inside the .exe: no DLL version conflicts
if(MINGW)
    target_link_options(ykw1 PRIVATE -static)
endif()
