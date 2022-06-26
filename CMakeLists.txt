cmake_minimum_required(VERSION 3.16)
project(WireMonitoring)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH /bb/wire_m)
set(LIBRARY_OUTPUT_PATH /bb/lib)

include_directories(include/)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src)

add_executable(wire_monitoring main.cpp)
add_dependencies(wire_monitoring WireMonitoring)

find_library(mysqlclient mysqlclient /usr/local/mysql/lib)
find_library(bb bb /bb/lib)
find_library(MysqlOrm MysqlOrm /bb/lib)
find_library(crypto crypto /bb/lib)
find_library(WireMonitoring WireMonitoring /bb/lib)
target_link_libraries(wire_monitoring ${bb} ${mysqlclient} ${MysqlOrm} ${crypto} ${WireMonitoring})

#ubuntu需要查找Threads第三方库,所以要添加以下两行代码
find_package(Threads)
target_link_libraries(wire_monitoring ${CMAKE_THREAD_LIBS_INIT})