aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_FILE)
#add_library(WM STATIC ${SRC_FILE})
add_library(WM SHARED ${SRC_FILE})