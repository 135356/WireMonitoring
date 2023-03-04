cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
project(bbhttp VERSION 5.0.0)
ADD_DEFINITIONS(-D_PROJECT_VERSION=\"${PROJECT_VERSION}\")

#Debug｜Release模式: -g表示带调试信息，-Wall表示输出所有的警告信息
set(CMAKE_BUILD_TYPE Release) #cmake根据编译器加-g参数(调试)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILE)
set(install_dir ${CMAKE_SOURCE_DIR}/build)
add_library(${PROJECT_NAME} STATIC ${SRC_FILE})

set(CMAKE_PREFIX_PATH ${install_dir}/cmake/modules)
find_package(bb 5.0 REQUIRED NO_MODULE)
find_package(mysql REQUIRED NO_MODULE)
find_package(mysql_orm REQUIRED NO_MODULE)
include_directories(${bb_INCLUDES} ${mysql_INCLUDES} ${mysql_orm_INCLUDES})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
add_executable(${PROJECT_NAME}_run main.cpp)
add_dependencies(${PROJECT_NAME}_run ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_run -lpthread bb mysql_orm ${PROJECT_NAME} ${mysql_LIBS})
set(CMAKE_INSTALL_PREFIX ${install_dir}/${PROJECT_NAME})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" TYPE INCLUDE)
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_run
    EXPORT Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(EXPORT Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION cmake
)
set(BIN bin)
set(CMAKE cmake)
set(INCLUDES include)
set(LIBS lib)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    Config.cmake.in
    ${install_dir}/cmake/modules/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${install_dir}/cmake/modules
    PATH_VARS BIN CMAKE INCLUDES LIBS
)
write_basic_package_version_file(
    ${install_dir}/cmake/modules/${PROJECT_NAME}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)