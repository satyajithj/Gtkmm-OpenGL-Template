cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project ("gtkmm_opengl")

message (STATUS "[${PROJECT_NAME}] Processing ${CMAKE_CURRENT_LIST_FILE}")

file(GLOB_RECURSE headers src/*.h)
file(GLOB_RECURSE textures textures/*)

# Setup target executable with the same name as our project
add_executable (${PROJECT_NAME}
    src/main.cpp

    src/cameras/cameramanager.cpp
    src/cameras/orthographicprojection.cpp
    src/cameras/perspectiveprojection.cpp

    src/shaders/shadermngr.cpp

    src/gui/customglarea.cpp
    src/gui/mainwindow.cpp
    # src/gui/tests/glonlymainwindow.cpp

    src/objects/crate.cpp

    extern/stb/stb_image.cpp

    ${headers}
)

# Find packages
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(EPOXY epoxy)

find_package (Threads REQUIRED)

# Set include directories
execute_process (
    COMMAND bash -c "find ../src ../extern -type d -print"
    OUTPUT_VARIABLE PROJECT_INCLUDE_DIRECTORIES
)

target_include_directories (${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern
    ${GTKMM_INCLUDE_DIRS}
    ${EPOXY_INCLUDE_DIRS}
)

# Link against libraries
target_link_libraries (${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    ${GTKMM_LIBRARIES}
    ${EPOXY_LIBRARIES}
)

# Set C++ standard to 14 (required for ids_peak)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
)