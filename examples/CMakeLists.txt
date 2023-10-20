include_directories(${resultpp_INCLUDE_DIRS})

add_executable(example main.cxx)
target_link_directories(example PRIVATE ${resultpp_INCLUDE_DIRS})
target_link_libraries(example PRIVATE resultpp)