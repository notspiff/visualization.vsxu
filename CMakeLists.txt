PROJECT(visualization.vsxu)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(kodi REQUIRED)
find_package(VSXu)
find_package(GLEW REQUIRED)

if(VSXU_FOUND)
  list(GET VSXU_INCLUDE_DIRS 0 vsxu_includes)
  get_filename_component(vsxu_path ${vsxu_includes} PATH)
  get_filename_component(vsxu_path ${vsxu_path} PATH)
  install(DIRECTORY  ${vsxu_path}/share/vsxu/visuals_faders
                     ${vsxu_path}/share/vsxu/visuals_player
         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kodi/addons/visualization.vsxu/resources)
else()
  message(STATUS "VSXu not found on system, building our own copy")
  include(ExternalProject)
  externalproject_add(vsxu
                      URL https://github.com/vovoid/vsxu/archive/0.5.1.tar.gz
                      PREFIX vsxu
                      PATCH_COMMAND patch -p1 < ${PROJECT_SOURCE_DIR}/depends/common/vsxu/01-fix-order.patch
                      CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                 -DVSXU_STATIC=1
                                 -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/vsxu
                                 -DCMAKE_POSITION_INDEPENDENT_CODE=1)

  if(KODI_BUILD_DIR)
    add_custom_command(TARGET vsxu POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_BINARY_DIR}/vsxu/share/vsxu/visuals_player
                               ${KODI_BUILD_DIR}/addons/visualization.vsxu/resources/visuals_player
                       COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_BINARY_DIR}/vsxu/share/vsxu/visuals_faders
                               ${KODI_BUILD_DIR}/addons/visualization.vsxu/resources/visuals_faders)
  endif()
  install(DIRECTORY  ${PROJECT_BINARY_DIR}/vsxu/share/vsxu/visuals_faders
                     ${PROJECT_BINARY_DIR}/vsxu/share/vsxu/visuals_player
         DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kodi/addons/visualization.vsxu/resources)

  set(VSXU_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/vsxu/include/vsxu
                        ${PROJECT_BINARY_DIR}/vsxu/include/vsxu/engine
                        ${PROJECT_BINARY_DIR}/vsxu/include/vsxu/engine_audiovisual
                        ${PROJECT_BINARY_DIR}/vsxu/include/vsxu/engine_graphics)

  set(VSXU_LIBRARIES -Wl,--whole-archive
                     -Lvsxu/lib
                     -lvsxu_engine
                     -lvsxu_engine_audiovisual
                     -lvsxu_engine_graphics
                     -lvsx_widget
                     -Lvsxu/lib/vsxu/plugins
                     -l:bitmap.generators.a
                     -l:bitmap.modifiers.a
                     -l:_deprecated.a
                     -l:lines.a
                     -l:math.a
                     -l:math.oscillators.a
                     -l:mesh.generators.a
                     -l:mesh.importers.a
                     -l:mesh.modifiers.a
                     -l:mesh.render.a
                     -l:opengl.a
                     -l:particlesystem.generators.a
                     -l:particlesystem.modifiers.a
                     -l:particlesystem.render.a
                     -l:render.basic.a
                     -l:render.glsl.a
                     -l:render.opengl.a
                     -l:render.skybox.a
                     -l:render.text.a
                     -l:selectors.a
                     -l:sound.rtaudio.a
                     -l:system.a
                     -l:texture.a
                     -Wl,--no-whole-archive)
endif()

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
