# - Try to find VSXu
# Once done this will define
#
# VSXU_FOUND - system has libvsxu
# VSXU_INCLUDE_DIRS - the libvsxu include directory
# VSXU_LIBRARIES - The libvsxu libraries

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules (VSXU libvsxu)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VSXU DEFAULT_MSG VSXU_INCLUDE_DIRS VSXU_LIBRARIES)

mark_as_advanced(VSXU_INCLUDE_DIRS VSXU_LIBRARIES)
