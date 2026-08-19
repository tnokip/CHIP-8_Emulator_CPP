cmake_minimum_required(VERSION 3.12)

project(Chip8
    VERSION 1.0
    LANGUAGES CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find SDL
find_package(SDL3 REQUIRED CONFIG)

#Sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

# Executable
add_executable(Chip8 ${SOURCES})

# Headers
target_include_directories(Chip8 PRIVATE
    include
)


# Link sdl3
target_link_libraries(Chip8 PRIVATE
    SDL3::SDL3
)

# Warnings
if(MSVC)
    target_compile_options(Chip8 PRIVATE /W4)
else()
    target_compile_options(Chip8 PRIVATE
        -Wall -Wextra -Wpedantic)
endif()

# Copy runtime dlls only on windows
if(WIN32)
    add_custom_command(TARGET Chip8 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:Chip8>
        $<TARGET_FILE_DIR:Chip8>
    COMMAND_EXPAND_LISTS
)
endif()

# Copy ROMS beside executable on every platform
add_custom_command(TARGET Chip8 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/c8roms
        $<TARGET_FILE_DIR:Chip8>/c8roms
)

