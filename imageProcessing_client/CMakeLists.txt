cmake_minimum_required(VERSION 3.10)
project(Win32App LANGUAGES CXX RC)

# Use Unicode
add_definitions(-DUNICODE -D_UNICODE)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RC_COMPILER "rc")

# Include headers
include_directories("${CMAKE_SOURCE_DIR}/includes")

# Source files
set(SOURCES
    window.cpp
    src/dialog_edit.cpp
    src/dialog_register.cpp
    src/dialog_about.cpp
    src/helpers.cpp
)

# Resource file (FIXED path)
set(RESOURCE_FILES
    Window.rc
)

# Explicitly mark RC file
set_source_files_properties(Window.rc PROPERTIES LANGUAGE RC)

# Define executable
add_executable(Win32App WIN32
    ${SOURCES}
    ${RESOURCE_FILES}
)

# Link necessary Windows libraries
target_link_libraries(Win32App
    user32
    gdi32
    comdlg32
    ole32
    winmm
)
