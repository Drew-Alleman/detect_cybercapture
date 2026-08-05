cmake_minimum_required(VERSION 3.16)
project(CyberCaptureDetector LANGUAGES CXX)

# Default to Release if the user didn't specify a build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
               "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Examples ---

# 1. Window-class detection (snxhk_border_mywnd_*)
add_executable(border_class examples/border_class.cpp)
target_link_libraries(border_class PRIVATE user32)

# 2. Memory string-scan detection (SnxHk_InstallHook)
add_executable(strings examples/strings.cpp)
target_link_libraries(strings PRIVATE user32)

# Optional: pure Windows GUI applications (no console window)
# set_target_properties(border_class strings PROPERTIES WIN32_EXECUTABLE TRUE)