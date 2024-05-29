cmake_minimum_required(VERSION 3.26)
project(NetCommandConnector)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_DEBUG_LIBS  OFF)
set(Boost_USE_RELEASE_LIBS ON)

find_package(Boost)

if(Boost_FOUND)
    message("Boost found!")
    include_directories( ${Boost_INCLUDE_DIRS} )
    link_directories(${Boost_LIBRARY_DIRS})
endif()

set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src" )

message( "SourceDir: ${PROJECT_SOURCE_DIR}" )
set(SRC_FILES ${PROJECT_SOURCE_DIR}/net/Endpoint.cpp
              ${PROJECT_SOURCE_DIR}/net/CommandConnector.cpp
              ${PROJECT_SOURCE_DIR}/net/CommandSender.cpp
              ${PROJECT_SOURCE_DIR}/net/ResponseHandler.cpp
              ${PROJECT_SOURCE_DIR}/net/ResponseExecutor.cpp
              ${PROJECT_SOURCE_DIR}/net/ResponseInterpreter.cpp
              ${PROJECT_SOURCE_DIR}/net/ResponseExecutionInterface.cpp
)

add_library(NetCommandConnector STATIC ${SRC_FILES})
target_link_libraries(NetCommandConnector ${Boost_LIBRARIES})