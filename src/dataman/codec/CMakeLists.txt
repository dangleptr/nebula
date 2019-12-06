cmake_minimum_required(VERSION 3.0.0)

message(STATUS "CMAKE_CURRENT_BINARY_DIR:" ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR:" ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "NEBULA_HOME:" ${NEBULA_HOME})

include_directories(AFTER ${NEBULA_HOME}/src)
include_directories(AFTER ${NEBULA_HOME}/src/common)

add_library(nebula_codec_obj
            OBJECT
            # NebulaCodecImpl.cpp
            Slice.cpp
            ${NEBULA_HOME}/src/common/base/Cord.cpp
            SchemaProviderIf.cpp
            NebulaSchemaProvider.cpp
            RowReader.cpp
            RowWriter.cpp
           )


target_link_libraries(nebula_codec
                      -static-libgcc
                      -static-libstdc++)

add_subdirectory(test)
