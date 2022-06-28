cmake_minimum_required(VERSION 3.16)
project(WireMonitoring)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH /bb/wire_monitoring)
set(LIBRARY_OUTPUT_PATH /bb/lib)

include_directories(include/)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src)

add_executable(WM_main main.cpp)
add_dependencies(WM_main WM)

find_library(mysqlclient mysqlclient /usr/local/mysql/lib)
find_library(bb bb /bb/lib)
find_library(MysqlOrm MysqlOrm /bb/lib)
find_library(crypto crypto /bb/lib)
find_library(WM WM /bb/lib)
target_link_libraries(WM_main ${bb} ${mysqlclient} ${MysqlOrm} ${crypto} ${WM})

#ubuntu需要查找Threads第三方库,所以要添加以下两行代码
find_package(Threads)
target_link_libraries(WM_main ${CMAKE_THREAD_LIBS_INIT})