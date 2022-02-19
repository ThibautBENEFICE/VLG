cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project( BE )
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=leak")
include_directories( /opt/local/include/igraph/
                    include/
                    src. )
link_directories(/opt/local/lib)
add_executable(be
                src/main.cc
                src/graph_treatment.cc
                src/file.cc)
target_link_libraries(be LINK_PUBLIC igraph)
