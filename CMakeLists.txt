cmake_minimum_required(VERSION 3.16)
project(plantah LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master)
FetchContent_MakeAvailable(SFML)

install(DIRECTORY assets/ DESTINATION bin/assets)

file(GLOB SOURCES "src/*.cpp")
add_executable(plantah ${SOURCES})
target_link_libraries(plantah PRIVATE sfml-graphics)
target_compile_features(plantah PRIVATE cxx_std_17)

if(WIN32)
    target_link_libraries(plantah PRIVATE ws2_32)
    set_target_properties(plantah PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -static")
    add_custom_command(
        TARGET plantah
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:plantah>
        VERBATIM)
endif()

install(TARGETS plantah)