
message(STATUS "CMAKE_CURRENT_BINARY_DIR:" ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR:" ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "NEBULA_HOME:" ${NEBULA_HOME})

# locate jni header
include_directories($ENV{JAVA_HOME}/include
                    $ENV{JAVA_HOME}/include/linux)
include_directories(AFTER ${NEBULA_HOME}/src)
include_directories(AFTER ${NEBULA_HOME}/src/common)

add_compile_options(-fPIC)

add_library(nebula_codec_obj
            OBJECT
            ${NEBULA_HOME}/src/common/base/Cord.cpp
            ${NEBULA_HOME}/src/dataman/codec/Slice.cpp
            ${NEBULA_HOME}/src/dataman/codec/SchemaProviderIf.cpp
            ${NEBULA_HOME}/src/dataman/codec/NebulaSchemaProvider.cpp
            ${NEBULA_HOME}/src/dataman/codec/RowReader.cpp
            ${NEBULA_HOME}/src/dataman/codec/RowWriter.cpp
           )

add_library(nebula_native_client
            SHARED
            $<TARGET_OBJECTS:nebula_codec_obj>
            src/main/cpp/com_vesoft_client_NativeClient.cpp)


target_link_libraries(nebula_native_client
                      -lglog
                      -lgflags
                      -static-libgcc
                      -static-libstdc++
                      -lpthread
                      -ldl)

