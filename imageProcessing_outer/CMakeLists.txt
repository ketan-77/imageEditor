cmake_minimum_required(VERSION 3.10)
project(imageProcessingOuterComponent LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source file
set(SOURCES
    imageProcessingOuterComponent.cpp
)

# Define DLL target
add_library(imageProcessingOuterComponent SHARED ${SOURCES})

# Link the .def file
set_target_properties(imageProcessingOuterComponent PROPERTIES
    LINK_FLAGS "/DEF:${CMAKE_SOURCE_DIR}/imageProcessingOuterComponent.def /SUBSYSTEM:WINDOWS"
)

# Link Windows libraries
target_link_libraries(imageProcessingOuterComponent
    user32
    ole32
    winmm
)
