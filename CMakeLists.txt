cmake_minimum_required(VERSION 3.22)
project(external_debug)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED python)
find_package(Python COMPONENTS Interpreter Development)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Python_INCLUDE_DIRS})

set(SHARED_SRCS shared/Controller.cpp shared/Controller.h)
set(MAIN_SRCS main/main.cpp main/DynamicLibrary.cpp)
set(DEBUGGER_SRCS
        debugger/debugger.cpp
        debugger/Interpreter.cpp
        debugger/Interpreter.h
        debugger/ControllerReference.cpp
        debugger/PseudoTerminal.cpp
        debugger/PseudoTerminal.h debugger/EndOfFileError.cpp debugger/EndOfFileError.h debugger/InputHistory.cpp debugger/InputHistory.h)

add_custom_target(shared_hash ALL
        COMMAND ${CMAKE_SOURCE_DIR}/hash_shared.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(main ${SHARED_SRCS} ${MAIN_SRCS})
add_library(debugger SHARED ${SHARED_SRCS} ${DEBUGGER_SRCS})

add_dependencies(main shared_hash)
add_dependencies(debugger shared_hash)

set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(main ${Boost_LIBRARIES} ${Python_LIBRARIES} dl)
target_link_libraries(debugger ${Python_LIBRARIES} ${Boost_LIBRARIES} util)