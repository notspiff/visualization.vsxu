project(glfw)

include(ExternalProject)

externalproject_add(glfw
                     URL ${PROJECT_SOURCE_DIR}
                     PATCH_COMMAND chmod 755 ${PROJECT_SOURCE_DIR}/compile.sh
                     CONFIGURE_COMMAND ${PROJECT_SOURCE_DIR}/compile.sh
                     BUILD_COMMAND make x11
                     INSTALL_COMMAND make x11-install PREFIX=${CMAKE_INSTALL_PREFIX}
                     BUILD_IN_SOURCE 1)

install(CODE "message(done)")
