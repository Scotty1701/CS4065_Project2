cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)

project(spam_api VERSION 0.0.1)

# Set up project variables
set(LIB_NAME spam_api)

# Set the install location as a subdir of the project root
set(CMAKE_INSTALL_PREFIX "../install")

# Get dependencies
include(FetchContent)

FetchContent_Declare(
    json-cpp
    GIT_REPOSITORY https://github.com/Scotty1701/jsoncpp.git
    GIT_TAG origin/correct_build_interfaces
)

FetchContent_MakeAvailable(json-cpp)

get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

# Create the library target
add_library(${LIB_NAME} STATIC src/spam_api.cpp)

# Set the public header(s) of the library
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "include/spam_api.hpp")

# Set the include directories
target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link the target to its dependent libraries
target_link_libraries(${LIB_NAME} PUBLIC jsoncpp_lib)

# Specify where to install targets and define the EXPORT target name
install(TARGETS ${LIB_NAME}
    EXPORT ${LIB_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

# Install the exported target
install(EXPORT ${LIB_NAME}Targets
    FILE "${LIB_NAME}Targets.cmake"
    NAMESPACE "${LIB_NAME}::"
    DESTINATION cmake
)

# Create the Config and Version files
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake"
    INSTALL_DESTINATION cmake
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install the Config and Version Files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake"
    DESTINATION cmake
)
