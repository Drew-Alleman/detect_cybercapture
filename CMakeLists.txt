cmake_minimum_required(VERSION 3.16)
project(CyberCaptureDetector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Save the provided code as main.cpp
add_executable(detect_cybercapture main.cpp)

# Required for EnumWindows / GetClassNameA / MessageBoxA
target_link_libraries(detect_cybercapture PRIVATE user32)