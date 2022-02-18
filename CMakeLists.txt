cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
project( BE )
include_directories( /usr/local/include/igraph/
                    include/
                    src. )
link_directories(/usr/local/lib)
add_executable(be
                src/main.cc
                src/graph_treatment.cc
                src/file.cc)
target_link_libraries(be LINK_PUBLIC igraph)
add_compile_options(-fsanitize=leak)
add_link_options(-fsanitize=leak)
