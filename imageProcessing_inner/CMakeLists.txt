cmake_minimum_required(VERSION 3.10)
project(imageProcessingInnerComponent LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source
set(SOURCES
    imageProcessingInnerComponent.cpp
)

# Define the DLL target
add_library(imageProcessingInnerComponent SHARED ${SOURCES})

# Link the DEF file
set_target_properties(imageProcessingInnerComponent PROPERTIES
    LINK_FLAGS "/DEF:${CMAKE_SOURCE_DIR}/imageProcessingInnerComponent.def"
)

# Link Windows libraries
target_link_libraries(imageProcessingInnerComponent
    user32
    winmm
)

# Set Windows subsystem if needed (optional)
# Note: Normally not needed for DLLs, but included per original script
set_target_properties(imageProcessingInnerComponent PROPERTIES
    LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS"
)
