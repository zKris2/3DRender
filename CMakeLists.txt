cmake_minimum_required(VERSION 3.12)
project(Rotating-Album)
set(CMAKE_CXX_STANDARD 17)

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include
)
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib
)

add_executable(Rotating-Album "main.cpp" "glad.c")

target_link_libraries(Rotating-Album glfw3.lib)