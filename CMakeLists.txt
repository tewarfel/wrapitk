project(WrapITK)

# check cmake version
# Require 2.4.2 for MATH and LIST commands (REMOVE_ITEM is not the same in
# 2.4.1)
# 2.4.5 is required to parse the if(POLICY) command.
cmake_minimum_required(VERSION 2.4.5 FATAL_ERROR)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
  if(POLICY CMP0011)
    cmake_policy(SET CMP0011 NEW)
  endif(POLICY CMP0011)

endif(COMMAND CMAKE_POLICY)
include("CMakeUtilityFunctions.cmake")

set(ITK_VERSION "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH}")
if(NOT "${CMAKE_VERSION}" STRLESS "2.6.3")
  cmake_policy(SET CMP0011 OLD)
endif(NOT "${CMAKE_VERSION}" STRLESS "2.6.3")

enable_testing()
include(CTest)
if(BUILD_TESTING)
  set(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  mark_as_advanced(BUILDNAME)
endif(BUILD_TESTING)

###############################################################################
# Config ITKWrap
###############################################################################

# on visual studio 2005 and later, we need to use /bigobj as the
# generated wrappings take up more than 65535 sections in some
# objects.
if(CMAKE_COMPILER_2005)
  add_definitions(/bigobj)
endif(CMAKE_COMPILER_2005)


# Set up options needed for ConfigureWrapping

# set WrapITK version
set(WrapITK_VERSION_MAJOR 0)
set(WrapITK_VERSION_MINOR 4)
set(WrapITK_VERSION_PATCH 0)


option(WRAP_unsigned_char "Wrap unsigned char type" OFF)
option(WRAP_unsigned_short "Wrap unsigned short type" ON)
option(WRAP_unsigned_long "Wrap unsigned long type" OFF)

option(WRAP_signed_char "Wrap signed char type" OFF)
option(WRAP_signed_short "Wrap signed short type" OFF)
option(WRAP_signed_long "Wrap signed long type" OFF)

option(WRAP_float "Wrap float type" ON)
option(WRAP_double "Wrap double type" OFF)

option(WRAP_vector_float "Wrap vector float type" ON)
option(WRAP_vector_double "Wrap vector double type" OFF)

option(WRAP_covariant_vector_float "Wrap covariant vector float type" ON)
option(WRAP_covariant_vector_double "Wrap covariant vector double type" OFF)

option(WRAP_rgb_unsigned_char "Wrap RGB< unsigned char > type" OFF)
option(WRAP_rgb_unsigned_short "Wrap RGB< unsigned short > type" ON)

option(WRAP_rgba_unsigned_char "Wrap RGBA< unsigned char > type" OFF)
option(WRAP_rgba_unsigned_short "Wrap RGBA< unsigned short > type" ON)

option(WRAP_complex_float "Wrap complex<float> type" ON)
option(WRAP_complex_double "Wrap complex<double> type" OFF)

set(WRAP_ITK_DIMS "2;3" CACHE STRING "dimensions available separated by semicolons (;)")

option(INSTALL_WRAP_ITK_COMPATIBILITY "Install FindWrapITK.cmake and WrapITK.pth (so cmake and python can find WrapITK) as part of the standard installation process. This requires admin privileges. These files can be separately installed by making the install_wrapitk_compatibility target. " ON)

set(WRAP_ITK_INSTALL_PREFIX "lib/InsightToolkit/WrapITK/" CACHE PATH "subpath where where most of WrapITK files will be installed")


# Output directories.
set(WRAP_ITK_CONFIG_DIR "${WrapITK_SOURCE_DIR}/")
set(WRAP_ITK_CMAKE_DIR "${WrapITK_SOURCE_DIR}")
set(WRAP_ITK_JAVA_DIR "${WrapITK_BINARY_DIR}/Java")
if(NOT LIBRARY_OUTPUT_PATH)
  set(LIBRARY_OUTPUT_PATH ${WrapITK_BINARY_DIR}/lib CACHE INTERNAL "Single output directory for building all libraries.")
endif(NOT LIBRARY_OUTPUT_PATH)
if(NOT EXECUTABLE_OUTPUT_PATH)
  set(EXECUTABLE_OUTPUT_PATH ${WrapITK_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
endif(NOT EXECUTABLE_OUTPUT_PATH)
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH WRAP_ITK_INSTALL_PREFIX)
set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

###############################################################################
# Configure CPack
###############################################################################

# CPack was introduced in cmake 2.4
set(CMAKE_VERSION "${CMAKE_CACHE_MAJOR_VERSION}.${CMAKE_CACHE_MINOR_VERSION}.${CMAKE_CACHE_RELEASE_VERSION}")
if("${CMAKE_VERSION}" STRGREATER "2.4")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Enhanced languages support for the Insight Toolkit")
  set(CPACK_PACKAGE_VENDOR "Gaetan Lehmann, Zachary Pincus")
  set(CPACK_PACKAGE_VERSION_MAJOR "${WrapITK_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${WrapITK_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${WrapITK_VERSION_PATCH}")
  # set(CPACK_PACKAGE_INSTALL_DIRECTORY "WrapITK-${WrapITK_VERSION_MAJOR}.${WrapITK_VERSION_MINOR}")
  # set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Documentation/Copyright/ITKCopyright.txt")
  include(CPack)
endif("${CMAKE_VERSION}" STRGREATER "2.4")

install(FILES
  CMakeUtilityFunctions.cmake
  ConfigureWrapping.cmake
  TypedefMacros.cmake
  WrapBasicTypes.cmake
  WrapITKTypes.cmake
  DESTINATION "${WRAP_ITK_INSTALL_PREFIX}/Configuration"
)

install(FILES "${WrapITK_BINARY_DIR}/InstallOnly/WrapITKConfig.cmake"
  DESTINATION "${WRAP_ITK_INSTALL_PREFIX}/"
)

# Add a custom target to drive installation of the WrapITK-compatibility files
if(INSTALL_WRAP_ITK_COMPATIBILITY)
  # if we should by default install the files, make a custom target that will
  # always be built.
  CREATE_INSTALL_AT_ABSOLUTE_PATH_TARGET(install_wrapitk_compatibility DEFAULT
    "Installing WrapITK compatibility files. Requires admin priviliges: turn off INSTALL_WRAP_ITK_COMPATIBILITY in cmake to disable.")
else(INSTALL_WRAP_ITK_COMPATIBILITY)
  # otherwise, add a target that will install the files only when that target is
  # manually built.
  CREATE_INSTALL_AT_ABSOLUTE_PATH_TARGET(install_wrapitk_compatibility ON_DEMAND
    "Installing WrapITK compatibility files. Requires admin priviliges.")
endif(INSTALL_WRAP_ITK_COMPATIBILITY)

INSTALL_AT_ABSOLUTE_PATH(install_wrapitk_compatibility "${CMAKE_ROOT}/Modules" "${WrapITK_BINARY_DIR}/InstallOnly/FindWrapITK.cmake")

set(WRAP_ITK_TYPEDEFS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Typedefs")
set(WRAP_ITK_LIB_DIRECTORY  "${LIBRARY_OUTPUT_PATH}")


###############################################################################
# The real work on wrappers
###############################################################################

include(ConfigureWrapping.cmake)


###############################################################################
# let the different languages running some code before begining to parse the
# modules
###############################################################################

WRAP_LIBRARIES()


###############################################################################
# Configure specific wrapper modules
###############################################################################

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Libraries" "${CMAKE_CURRENT_BINARY_DIR}/Libraries")


###############################################################################
# let the different languages running some code after have parsed all the
# modules
###############################################################################

END_WRAP_LIBRARIES()

###############################################################################
# Configure WrapITKConfig.cmake and FindWrapITK.cmake for external projects
###############################################################################
# First, configure for use directly from the build dir.
# we must add the options for the languages for the build dir case, so the
# external project won't find all the languages, even the usued ones.
# In the installed case, this feature is useful to let the packagers make several
# subpackages for wrapitk.
set(CONFIG_WRAP_ITK_MORE_OPTIONS)
foreach(lang ${WRAP_ITK_LANGUAGES} ${WRAP_ITK_UNUSED_LANGUAGES})
  string(TOUPPER "${lang}" lang_upper)
  set(CONFIG_WRAP_ITK_MORE_OPTIONS "${CONFIG_WRAP_ITK_MORE_OPTIONS}set(WRAP_ITK_${lang_upper} ${WRAP_ITK_${lang_upper}} CACHE BOOL \"Build swig ${lang} wrapper support.\")\n")
endforeach(lang)
set(CONFIG_CONFIG_DIR "${WrapITK_SOURCE_DIR}/ConfigurationInputs")
set(CONFIG_CMAKE_DIR "${WrapITK_SOURCE_DIR}")
set(CONFIG_WRAP_ITK_TYPEDEFS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Typedefs")
set(CONFIG_WRAP_ITK_INSTALL_LOCATION "${CMAKE_INSTALL_PREFIX}/${WRAP_ITK_INSTALL_PREFIX}")
set(CONFIG_WRAP_ITK_MODULES "${WRAP_ITK_MODULES}")
set(CONFIG_WRAP_ITK_PYTHON_MODULE "${WrapITK_BINARY_DIR}/Python/\\\${CTEST_CONFIGURATION_TYPE}")
set(CONFIG_WRAP_ITK_PYTHON_LIBS "${LIBRARY_OUTPUT_PATH}/\\\${CTEST_CONFIGURATION_TYPE}")
set(CONFIG_WRAP_ITK_LIB_DIR  "${LIBRARY_OUTPUT_PATH}")
set(CONFIG_WRAP_ITK_IGENERATOR  "${WrapITK_BINARY_DIR}/Languages/SwigInterface/igenerator.py")
configure_file("${WRAP_ITK_CONFIG_DIR}/WrapITKConfig.cmake.in"
               "${WrapITK_BINARY_DIR}/WrapITKConfig.cmake"
               @ONLY IMMEDIATE)

set(CONFIG_WRAP_ITK_DIR "${WrapITK_BINARY_DIR}")
configure_file("${WRAP_ITK_CONFIG_DIR}/FindWrapITK.cmake.in"
               "${WrapITK_BINARY_DIR}/FindWrapITK.cmake"
               @ONLY IMMEDIATE)

# Second, configure for installed use. Ensure that no absolute paths
# go into the installed config file, so that it is as portable as possible.
set(CONFIG_WRAP_ITK_MORE_OPTIONS)
set(CONFIG_CONFIG_DIR "\${WrapITK_DIR}/Configuration/Inputs")
set(CONFIG_CMAKE_DIR "\${WrapITK_DIR}/Configuration")
set(CONFIG_WRAP_ITK_TYPEDEFS_DIRECTORY "\${WrapITK_DIR}/Configuration/Typedefs")
set(CONFIG_WRAP_ITK_INSTALL_LOCATION "\${WrapITK_DIR}")
set(CONFIG_WRAP_ITK_PYTHON_MODULE "\${WrapITK_DIR}/Python/")
set(CONFIG_WRAP_ITK_PYTHON_LIBS "\${WrapITK_DIR}/lib")
set(CONFIG_WRAP_ITK_LIB_DIR  "\${WrapITK_DIR}/lib")
set(CONFIG_WRAP_ITK_IGENERATOR  "\${WrapITK_DIR}/Configuration/Languages/SwigInterface/igenerator.py")
configure_file("${WRAP_ITK_CONFIG_DIR}/WrapITKConfig.cmake.in"
               "${WrapITK_BINARY_DIR}/InstallOnly/WrapITKConfig.cmake"
               @ONLY IMMEDIATE)

set(CONFIG_WRAP_ITK_DIR "${CMAKE_INSTALL_PREFIX}/${WRAP_ITK_INSTALL_PREFIX}")
configure_file("${WRAP_ITK_CONFIG_DIR}/FindWrapITK.cmake.in"
               "${WrapITK_BINARY_DIR}/InstallOnly/FindWrapITK.cmake"
               @ONLY IMMEDIATE)
