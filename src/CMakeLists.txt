configure_file(SimplePipeConfig.h.in SimplePipeConfig.h)

add_library(simplepipe_lib STATIC 
	SimplePipe.cpp SimplePipe.h SimplePipeConfig.h.in)

# To be able to include generated SimplePipeConfig.h
target_include_directories(simplepipe_lib PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(simplepipe main.cpp)
target_link_libraries(simplepipe PUBLIC simplepipe_lib)