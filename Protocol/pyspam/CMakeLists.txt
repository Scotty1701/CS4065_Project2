cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(pyspam)

# Get dependencies
include(FetchContent)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git   
    GIT_TAG origin/master
)

FetchContent_MakeAvailable(pybind11)

find_package(spam_api REQUIRED
    PATHS "../install/cmake"
)

# Set up sources
pybind11_add_module(pyspam pyspam.cpp)

target_link_libraries(pyspam PUBLIC jsoncpp_lib spam_api::spam_api)

install(TARGETS pyspam
    COMPONENT python
    LIBRARY DESTINATION pyspam
)

if (WIN32)
  install(FILES "../install/bin/jsoncpp.dll"
      DESTINATION pyspam
  )
endif (WIN32)
if (UNIX)
  install(FILES "../install/lib64/libjsoncpp.so"
      DESTINATION pyspam
  )
endif (UNIX)
install(FILES "./pyproject.toml"
  DESTINATION .
)
install(FILES "./__init__.py"
  DESTINATION pyspam
)
