cmake_minimum_required(VERSION 3.16)
project(WireMonitoring)

set(CMAKE_CXX_STANDARD 14)
SET(EXECUTABLE_OUTPUT_PATH /bb/wire_m)

add_executable(wire_m main.cpp)

include_directories(include/)

find_library(mysqlclient mysqlclient /usr/local/mysql/lib)
find_library(bb bb /bb/lib)
find_library(MysqlOrm MysqlOrm /bb/lib)
find_library(crypto crypto /bb/lib)
target_link_libraries(wire_m ${bb} ${mysqlclient} ${MysqlOrm} ${crypto})

#ubuntu需要查找Threads第三方库,所以要添加以下两行代码
find_package(Threads)
target_link_libraries(wire_m ${CMAKE_THREAD_LIBS_INIT})