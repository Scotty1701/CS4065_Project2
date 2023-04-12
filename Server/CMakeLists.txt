cmake_minimum_required(VERSION 3.24)

project(message_server VERSION 0.0.1)

set(LIB_NAME easy_winsock)

add_library(${LIB_NAME} STATIC src/easy_winsock.cpp)

set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "include/easy_winsock.hpp")

# Set the include directories
target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Specify where to install targets and define the EXPORT target name
install(TARGETS ${LIB_NAME}
    #EXPORT ${LIB_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)
