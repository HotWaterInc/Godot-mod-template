cmake_minimum_required(VERSION 3.6)
project(template)

set(PROJECT_NAME "Template")
set(PROJECT_NAME_LOWER_CASE "template")

option(BUILD_FOR_EDITOR "If true, create and install libs as release ready to use in the demo folder" FALSE)


set(GODOT_GDEXTENSION_DIR godot-cpp/gdextension/ CACHE STRING "Path to GDExtension interface header directory")
set(CPP_BINDINGS_PATH godot-cpp CACHE STRING "Path to C++ bindings")

if (DEFINED BUILD_FOR_EDITOR)
    if (${BUILD_FOR_EDITOR})
        set(CMAKE_BUILD_TYPE Release)
        message("Build for editor mode ignore CMAKE_BUILD_TYPE, " CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")
        execute_process(
                COMMAND cmake -DCMAKE_BUILD_TYPE=Release godot-cpp -B godot-cpp
                WORKING_DIRECTORY "."
                COMMAND_ECHO STDOUT
        )
        execute_process(
                COMMAND cmake --build godot-cpp
                WORKING_DIRECTORY "."
                COMMAND_ECHO STDOUT
        )
    endif ()
endif ()

# OS specific
set(TARGET_PATH x11)
set(LIB_EXT ".so")

# Change the output directory to the bin directory
set(BUILD_PATH ${CMAKE_SOURCE_DIR}/demo/addons/${PROJECT_NAME_LOWER_CASE}/bin/${TARGET_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BUILD_PATH}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BUILD_PATH}")

# Set the c++ standard to c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GODOT_COMPILE_FLAGS)
set(GODOT_LINKER_FLAGS)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(GODOT_LINKER_FLAGS "-Wl")
else ()
    set(GODOT_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-R,'$$ORIGIN'")
endif ()

# Hmm.. maybe to strikt?
set(GODOT_COMPILE_FLAGS "-fPIC -g -Wwrite-strings")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wchar-subscripts -Wcomment -Wdisabled-optimization")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wformat -Wformat=2 -Wformat-security -Wformat-y2k")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wimport -Winit-self -Winline -Winvalid-pch -Werror")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wmissing-braces -Wmissing-format-attribute")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wpointer-arith")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wredundant-decls -Wreturn-type -Wsequence-point")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wswitch -Wswitch-enum -Wtrigraphs")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused-label")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wunused-value -Wvariadic-macros -Wvolatile-register-var -Wno-error=attributes")
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -Wno-ignored-attributes")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -fno-omit-frame-pointer -O0")
else ()
    set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -O3")
endif (CMAKE_BUILD_TYPE MATCHES Debug)

# Disable exception handling. Godot doesn't use exceptions anywhere, and this
# saves around 20% of binary size and very significant build time (GH-80513).
option(GODOT_DISABLE_EXCEPTIONS OFF "Force disabling exception handling code")
if (GODOT_DISABLE_EXCEPTIONS)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -D_HAS_EXCEPTIONS=0")
    else ()
        set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -fno-exceptions")
    endif ()
else ()
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /EHsc")
    endif ()
endif ()

# Get Sources
file(GLOB_RECURSE SOURCES src/*.c**)
file(GLOB_RECURSE HEADERS include/*.h**)

# Define our godot-cpp library
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} SYSTEM
        PRIVATE
        ${CPP_BINDINGS_PATH}/include
        ${CPP_BINDINGS_PATH}/gen/include
        ${GODOT_GDEXTENSION_DIR}
)

# Create the correct name (godot.os.build_type.system_bits)
# Synchronized with godot-cpp's CMakeLists.txt

set(BITS 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITS 64)
endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(GODOT_CPP_BUILD_TYPE Debug)
else ()
    set(GODOT_CPP_BUILD_TYPE Release)
endif ()

string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME)
string(TOLOWER ${GODOT_CPP_BUILD_TYPE} BUILD_TYPE)

#find_package(OpenSSL REQUIRED)
#find_package(PahoMqttCpp REQUIRED)
target_link_directories(${PROJECT_NAME}
        PRIVATE
        ${CPP_BINDINGS_PATH}/bin/
)
target_link_libraries(${PROJECT_NAME}
        libgodot-cpp.${SYSTEM_NAME}.${BUILD_TYPE}$<$<NOT:$<PLATFORM_ID:Android>>:.${BITS}>.a
)

# Add the compile flags
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS ${GODOT_COMPILE_FLAGS})
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS ${GODOT_LINKER_FLAGS})
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME "${PROJECT_NAME_LOWER_CASE}.debug")
else ()
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME "${PROJECT_NAME_LOWER_CASE}.release")
endif ()

if (${BUILD_FOR_EDITOR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_PATH}/lib${PROJECT_NAME_LOWER_CASE}.release${LIB_EXT} ${BUILD_PATH}/lib${PROJECT_NAME_LOWER_CASE}.debug${LIB_EXT}
    )
endif ()