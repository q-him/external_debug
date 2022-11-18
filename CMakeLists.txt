cmake_minimum_required(VERSION 3.23)
project(external_debug)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED python)
find_package(Python COMPONENTS Interpreter Development)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Python_INCLUDE_DIRS})
message("boost lib: ${Boost_LIBRARIES}")
message("boost inc:${Boost_INCLUDE_DIR}")

set(shared_srcs shared/Controller.cpp)
set(main_srcs main/main.cpp main/DynamicLibrary.cpp debugger/PseudoTerminal.cpp)
set(debugger_srcs debugger/debugger.cpp)

add_executable(main ${shared_srcs} ${main_srcs})
add_library(debugger SHARED ${shared_srcs} ${debugger_srcs})

set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(main ${Boost_LIBRARIES} ${Python_LIBRARIES} dl)
target_link_libraries(debugger ${Python_LIBRARIES} ${Boost_LIBRARIES} util)