PROJECT(visualization.vsxu)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(kodi REQUIRED)
find_package(VSXu REQUIRED)

include_directories(${KODI_INCLUDE_DIR}
                    ${VSXU_INCLUDE_DIRS})

set(DEPLIBS ${VSXU_LIBRARIES})

set(VSXU_SOURCES src/addon.cpp)

build_addon(visualization.vsxu VSXU DEPLIBS)

include(CPack)
