cmake_minimum_required(VERSION 3.12)
project(3DRender)
set(CMAKE_CXX_STANDARD 17)

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include
)
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib
)

file(GLOB ASSETS "assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(Application)

add_executable(3DRender "main.cpp" "glad.c")

target_link_libraries(3DRender glfw3.lib application assimp-vc143-mtd.lib)