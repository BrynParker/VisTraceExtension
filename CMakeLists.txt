cmake_minimum_required (VERSION 3.20)

project("SunRayTracing" CXX)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_library(
	${PROJECT_NAME} SHARED
	"source/Main.cpp"
)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	"libs/VisTrace/include"
	"libs/gmod-module-base/include"
)
