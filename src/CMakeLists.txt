aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_FILE)
#add_library(WireMonitoring STATIC ${SRC_FILE})
add_library(WireMonitoring SHARED ${SRC_FILE})