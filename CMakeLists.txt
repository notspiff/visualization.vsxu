PROJECT(visualization.vsxu)

CMAKE_MINIMUM_REQUIRED(VERSION 3.1) # To get proper pkg-config behavior

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(Kodi REQUIRED)
find_package(VSXu REQUIRED)
find_package(GLEW REQUIRED)

list(GET VSXU_INCLUDE_DIRS 0 vsxu_includes)
get_filename_component(vsxu_path ${vsxu_includes} PATH)
get_filename_component(vsxu_path ${vsxu_path} PATH)
install(DIRECTORY  ${vsxu_path}/share/vsxu/visuals_faders
                   ${vsxu_path}/share/vsxu/visuals_player
       DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kodi/addons/visualization.vsxu/resources)

include_directories(${KODI_INCLUDE_DIR}
                    ${VSXU_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS})

# Need all flags
set(DEPLIBS ${VSXU_LDFLAGS} ${VSXU_LIBRARIES} ${GLEW_LIBRARIES})

set(VSXU_SOURCES src/addon.cpp)

build_addon(visualization.vsxu VSXU DEPLIBS)
if(NOT VSXU_FOUND)
  add_dependencies(visualization.vsxu vsxu)
endif()

include(CPack)
