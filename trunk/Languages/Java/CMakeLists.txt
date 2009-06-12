###############################################################################

SET(WRAP_ITK_JAVA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "java source dir")
SET(WRAP_ITK_JAVA_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "java binary dir")

# java stuff
FIND_PACKAGE(Java)
FIND_PACKAGE(JNI)
INCLUDE_DIRECTORIES(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})

# create the java directory in the classindex dir
FILE(MAKE_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java)

# some glorbal variables
SET(WRAP_ITK_JAVA_OUT_DIR ${PROJECT_BINARY_DIR}/Languages/Java/Proxies CACHE INTERNAL "Java proxies directory")
SET(WRAP_ITK_JAVA_SRC_DIR ${WRAP_ITK_JAVA_OUT_DIR}/src CACHE INTERNAL "Java proxies source directory")
#SET(WRAP_ITK_JAVA_BIN_DIR ${WRAP_ITK_JAVA_OUT_DIR}/bin CACHE INTERNAL "Java proxies binary directory")
SET(WRAP_ITK_JAVA_BIN_DIR ${WRAP_ITK_JAVA_SRC_DIR} CACHE INTERNAL "Java proxies binary directory")
SET(WRAP_ITK_JAVA_JAR_DIR ${LIBRARY_OUTPUT_PATH} CACHE INTERNAL "Java jar files directory")
SET(WRAP_ITK_JAVA_CLASS_NAMES "" CACHE INTERNAL "")
SET(WRAP_ITK_JAVA_JAR_DEPS "" CACHE INTERNAL "")
SET(WRAP_ITK_JAVA_USER_CLASS_PATH "" CACHE INTERNAL "additional user java classpath")

# java proxy package
SET(WRAP_ITK_JAVA_CORE_PACKAGE_NAME "org.itk" CACHE INTERNAL "")
SET(WRAP_ITK_JAVA_CORE_PACKAGE_PATH "org/itk" CACHE INTERNAL "") #TODO: this should be automated

# TODO: iterators are not supported by java
REMOVE(JAVA_ITK_MODULES "${WRAP_ITK_MODULES}" Iterators)

###############################################################################
# Install java stuff for external projects
IF(NOT EXTERNAL_WRAP_ITK_PROJECT)
  WRAP_ITK_INSTALL(/Configuration/Languages/Java
    CMakeLists.txt
    java.i
    module_ext.i.in
    manifest.in
  )
ENDIF(NOT EXTERNAL_WRAP_ITK_PROJECT)


###############################################################################
MACRO(CREATE_JAVA_DIRECTORY_STRUCTURE)

  MESSAGE(STATUS "Creating wrapping Java wrapping directories.")
  
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_OUT_DIR})
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_SRC_DIR})
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_BIN_DIR})
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_JAR_DIR})

  # Packages output directory
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_SRC_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH})

ENDMACRO(CREATE_JAVA_DIRECTORY_STRUCTURE)


#-----------------------------------------------------------------------------#
# call the initialisation macros
CREATE_JAVA_DIRECTORY_STRUCTURE()
#-----------------------------------------------------------------------------#


###############################################################################
MACRO(WRAP_LIBRARY_JAVA library_name)
#  SET(WRAP_ITK_JAVA_CONFIGURATION_TEMPLATES "")
#  SET(WRAP_ITK_JAVA_LIBRARY_MODULE "")
  SET(WRAP_ITK_JAVA_LIBRARY_DEPS )
#  SET(WRAP_ITK_JAVA_LIBRARY_DECLS )
#  SET(WRAP_ITK_JAVA_LIBRARY_CALLS )
  SET(WRAP_ITK_JAVA_CXX_FILES )
ENDMACRO(WRAP_LIBRARY_JAVA library_name)


###############################################################################
MACRO(END_WRAP_LIBRARY_JAVA)

  # Loop over the extra swig input files and add them to the generated files 
  # lists. Guess that the generated cxx output will have the same name as 
  # the .i input file.
  FOREACH(source ${WRAPPER_LIBRARY_SWIG_INPUTS})
    GET_FILENAME_COMPONENT(base_name ${source} NAME_WE)
    STRING(REPLACE "wrap_" "" base_name "${base_name}")
    WRAP_MODULE_JAVA("${base_name}")
    END_WRAP_MODULE_JAVA("${base_name}")
  ENDFOREACH(source)
  
  # set the package name and path
  STRING(TOLOWER ${WRAPPER_LIBRARY_NAME} _java_package_name)
  SET(java_package_path ${WRAP_ITK_JAVA_SRC_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH}/${_java_package_name})
  SET(java_package_bin_path ${WRAP_ITK_JAVA_BIN_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH}/${_java_package_name})
  SET(java_package_name ${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${_java_package_name})
  SET(java_jar_file_name ${java_package_name}.jar)
  SET(java_jar_file_path ${WRAP_ITK_JAVA_JAR_DIR}/${java_jar_file_name})
  
  # make the package directory
  FILE(MAKE_DIRECTORY ${java_package_path})
  
  # set the java source and bin files
  SET(java_file )
  SET(java_bin_file )
  FOREACH(class_name ${WRAP_ITK_JAVA_CLASS_NAMES})
    SET(java_file ${java_file} ${java_package_path}/${class_name}.java)
    SET(java_bin_file ${java_bin_file} ${java_package_bin_path}/${class_name}.class)
  ENDFOREACH(class_name ${WRAP_ITK_JAVA_CLASS_NAMES})
    
  CONFIGURE_FILE("${WRAP_ITK_JAVA_SOURCE_DIR}/module_ext.i.in"
    "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${WRAPPER_LIBRARY_NAME}_ext.i"
    @ONLY IMMEDIATE)
  WRAP_ITK_INSTALL(/Configuration/Typedefs/java
    "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${WRAPPER_LIBRARY_NAME}_ext.i")

  SET(interface_file "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${WRAPPER_LIBRARY_NAME}.i")

#  SET(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${WRAPPER_LIBRARY_NAME}Java.cpp")
  

#  ADD_CUSTOM_COMMAND(
#    OUTPUT ${cpp_file} ${java_file}
#    COMMAND ${SWIG_EXECUTABLE} -c++ -java -O -features autodoc=1
##    -Werror
#    -package ${java_package_name}
##       -fcompact
#    -w508 -w312 -w314 -w509 -w302 -w362
#    -w389 # operator[], to be suppressed later...
#    -w384 -w383 # operator++ ane operator--
#    -w361 # operator!
#    -w467 # overloaded functions (with typemaps)
#    -w401 -w503 -w516 #!
#    -o ${cpp_file}
#    -I${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/Java
#    -outdir ${java_package_path}
#    ${interface_file}
#    WORKING_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}
#    DEPENDS ${DEPS} ${WRAP_ITK_JAVA_LIBRARY_DEPS} ${interface_file} ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${WRAPPER_LIBRARY_NAME}_ext.i ${WRAP_ITK_JAVA_SOURCE_DIR}/java.i ${SWIG_EXECUTABLE}
#    COMMENT "Generating Java wrap files for class ${java_package_name}.${WRAPPER_LIBRARY_NAME}"
#  )

  
  # create java wrapping binaries
  SET(lib ${WRAPPER_LIBRARY_NAME}Java)
  INCLUDE_DIRECTORIES(${WRAPPER_LIBRARY_INCLUDE_DIRECTORIES})
  ADD_LIBRARY(${lib} MODULE ${WRAP_ITK_JAVA_CXX_FILES} ${WRAPPER_LIBRARY_CXX_SOURCES})
#  SET_TARGET_PROPERTIES(${lib} PROPERTIES PREFIX "_")
  IF(APPLE)
    SET_TARGET_PROPERTIES(${lib} PROPERTIES SUFFIX .jnilib)
  ENDIF(APPLE)
  # work around linkage problem on some solaris systems
  IF(CMAKE_SYSTEM MATCHES "SunOS-." AND CMAKE_COMPILER_IS_GNUCXX AND CMAKE_COMPILER_IS_GNUCC)
    TARGET_LINK_LIBRARIES(${lib} stdc++)
  ENDIF(CMAKE_SYSTEM MATCHES "SunOS-." AND CMAKE_COMPILER_IS_GNUCXX AND CMAKE_COMPILER_IS_GNUCC)
  # strict-aliasing breaks produced code with gcc, according to http://www.swig.org/Doc1.3/Java.html#compiling_dynamic
  IF(CMAKE_COMPILER_IS_GNUCC)
    SET_TARGET_PROPERTIES(${lib} PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
  ENDIF(CMAKE_COMPILER_IS_GNUCC)  
  TARGET_LINK_LIBRARIES(${lib} ${WRAPPER_LIBRARY_LINK_LIBRARIES})
  ADD_DEPENDENCIES(${lib} ${WRAPPER_LIBRARY_NAME}Swig)
  INSTALL(TARGETS "${lib}" DESTINATION "${WRAP_ITK_INSTALL_PREFIX}/lib")
  IF(EXTERNAL_WRAP_ITK_PROJECT)
    # don't depends on the targets from wrapitk in external projects
    FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS} ITKJavaBase)
      ADD_DEPENDENCIES(${lib} ${dep}Swig)
    ENDFOREACH(dep)
  ENDIF(EXTERNAL_WRAP_ITK_PROJECT)
  
  IF(WIN32)
    SET(sep "\;")
  ELSE(WIN32)
    SET(sep ":")
  ENDIF(WIN32)
  
  SET(class_path )
  # Current module source path
  SET(class_path "${class_path}${java_package_path}${sep}")
  # Dependency modules bin path
  #SET(class_path "${class_path}${WRAP_ITK_JAVA_BIN_DIR}${sep}")
  # Dependency modules jar files
  # manifest content
  SET(WRAP_ITK_JAVA_MANIFEST )
  FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS} ITKJavaBase)
    # create the import package commands
    STRING(TOLOWER ${dep} ldep)
    SET(dep_jar_file ${WRAP_ITK_LIB_DIRECTORY}/${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${ldep}.jar)
    SET(class_path "${class_path}${dep_jar_file}${sep}")
    SET(WRAP_ITK_JAVA_MANIFEST "${WRAP_ITK_JAVA_MANIFEST} ${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${ldep}.jar")
  ENDFOREACH(dep)
  FOREACH(ucp ${WRAP_ITK_JAVA_USER_CLASS_PATH})
    SET(class_path "${class_path}${ucp}${sep}")
  ENDFOREACH(ucp)

  # create the manifest file  
  IF(WRAP_ITK_JAVA_MANIFEST)
    SET(WRAP_ITK_JAVA_MANIFEST "Class-Path: ${WRAP_ITK_JAVA_MANIFEST}")
  ENDIF(WRAP_ITK_JAVA_MANIFEST)
  CONFIGURE_FILE("${WRAP_ITK_JAVA_SOURCE_DIR}/manifest.in"
                "${CMAKE_CURRENT_BINARY_DIR}/manifest"
                @ONLY IMMEDIATE)

  # Add java proxy compilation stuff
  # Only add this if there is any classes to be built
  IF(NOT "${WRAP_ITK_JAVA_CLASS_NAMES}" STREQUAL "")
    # compile java proxies
    SET(lib_jar ${lib}Jar)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${java_bin_file}
      DEPENDS ${java_file}
      COMMAND ${JAVA_COMPILE}
      ARGS
      -classpath "${class_path}"
      -d "${WRAP_ITK_JAVA_BIN_DIR}"
#      -sourcepath "${java_package_path}"
#      -verbose
      "${java_package_path}/*.java"
      COMMENT "Compiling java proxies: ${java_package_name}"
      )

    ADD_CUSTOM_COMMAND(
      OUTPUT ${java_jar_file_path}
      DEPENDS ${java_bin_file} ${CMAKE_CURRENT_BINARY_DIR}/manifest
      COMMAND ${JAVA_ARCHIVE}
      ARGS
      -cmf "${CMAKE_CURRENT_BINARY_DIR}/manifest" "${java_jar_file_path}"
      "${WRAP_ITK_JAVA_CORE_PACKAGE_PATH}/${_java_package_name}/*"
      WORKING_DIRECTORY ${WRAP_ITK_JAVA_BIN_DIR}
      COMMENT "Creating java archive: ${java_jar_file_name}"
      )
    ADD_CUSTOM_TARGET(${lib_jar} ALL DEPENDS ${java_bin_file} ${java_jar_file_path})
    WRAP_ITK_INSTALL(/lib ${java_jar_file_path})

    ADD_DEPENDENCIES(${lib_jar} ${lib})
    IF(NOT EXTERNAL_WRAP_ITK_PROJECT)
      # don't depends on the targets from wrapitk in external projects
      FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS} ITKJavaBase)
        ADD_DEPENDENCIES(${lib_jar} ${dep}Java)
        ADD_DEPENDENCIES(${lib_jar} ${dep}JavaJar)
      ENDFOREACH(dep)
    ENDIF(NOT EXTERNAL_WRAP_ITK_PROJECT)
  
    SET(WRAP_ITK_JAVA_JAR_DEPS ${WRAP_ITK_JAVA_JAR_DEPS} ${lib_jar})
  ELSE(NOT "${WRAP_ITK_JAVA_CLASS_NAMES}" STREQUAL "")
    MESSAGE("Warning: Unable to create java proxies for ${WRAPPER_LIBRARY_NAME}, nothing to get wrapped in this module.")
  ENDIF(NOT "${WRAP_ITK_JAVA_CLASS_NAMES}" STREQUAL "")

ENDMACRO(END_WRAP_LIBRARY_JAVA)

###############################################################################
MACRO(END_WRAP_MODULE_JAVA group_name)
  SET(WRAP_ITK_JAVA_CLASS_NAMES ${WRAP_ITK_JAVA_CLASS_NAMES} ${group_name}Java)
  
  # set the package name and path
  STRING(TOLOWER ${WRAPPER_LIBRARY_NAME} java_package_name)
  SET(java_package_path ${WRAP_ITK_JAVA_SRC_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH}/${java_package_name})
  SET(java_package_name ${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${java_package_name})
  
  # set the java source files
  SET(java_file )
  FOREACH(class_name ${WRAP_ITK_JAVA_CLASS_NAMES})
    SET(java_file ${java_file} ${java_package_path}/${class_name}.java)
  ENDFOREACH(class_name ${WRAP_ITK_JAVA_CLASS_NAMES})
  
  SET(base_name wrap_${group_name})
    
  # create the swig interface for all the groups in the module
  SET(interface_file "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${base_name}.i" )
  SET(lib ${group_name}Java)
  SET(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${base_name}Java.cpp")

  SET(WRAP_ITK_JAVA_CXX_FILES ${WRAP_ITK_JAVA_CXX_FILES} ${cpp_file})
  
  # prepare dependencies
  SET(DEPS )
  SET(WRAP_ITK_JAVA_TYPEMAP_JAVAIMPORTS )
  FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS} ITKJavaBase)
    # library dependencies
    SET(DEPS ${DEPS} ${${dep}SwigFiles})
    
    # create the import package commands
    STRING(TOLOWER ${dep} java_import_package_name)
    SET(java_import_package_name ${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${java_import_package_name})
    SET(WRAP_ITK_JAVA_TYPEMAP_JAVAIMPORTS ${WRAP_ITK_JAVA_TYPEMAP_JAVAIMPORTS} "import ${java_import_package_name}.*;")
  ENDFOREACH(dep)
  
  # if this is for an external library, let the user add extra swig args
  IF(EXTERNAL_WRAP_ITK_PROJECT)
    SET(WRAP_ITK_SWIG_ARGS_JAVA "" CACHE STRING "Extra user-defined swig arguments to be to the swig executable.")
  ENDIF(EXTERNAL_WRAP_ITK_PROJECT)
  
  ADD_CUSTOM_COMMAND(
    OUTPUT ${cpp_file} ${java_file}
    COMMAND ${SWIG_EXECUTABLE} -c++ -java -O -features autodoc=1
#    -Werror
    -package ${java_package_name}
#       -fcompact
    -w508 -w312 -w314 -w509 -w302 -w362
    -w389 # operator[], to be suppressed later...
    -w384 -w383 # operator++ ane operator--
    -w361 # operator!
    -w467 # overloaded functions (with typemaps)
    -w401 -w503 -w516 #!
    -w303 # extend itklightobject
    -o ${cpp_file}
    -I${LANGUAGES_SRC_DIR}
    -I${WRAP_ITK_JAVA_SOURCE_DIR}
    -I${WRAP_ITK_TYPEDEFS_DIRECTORY}/java
    -I${WRAP_ITK_TYPEDEFS_DIRECTORY}
    ${WRAP_ITK_SWIG_ARGS_JAVA}
    -outdir ${java_package_path}
    ${interface_file}
    WORKING_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java
    DEPENDS ${DEPS} ${interface_file} ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${base_name}_ext.i ${WRAP_ITK_JAVA_SOURCE_DIR}/java.i ${SWIG_EXECUTABLE}
    COMMENT "Generating Java wrap files for class ${java_package_name}.${group_name}"
  )
  # ${WRAP_ITK_JAVA_LIBRARY_DEPS} 
  
  CONFIGURE_FILE("${WRAP_ITK_JAVA_SOURCE_DIR}/module_ext.i.in"
    "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/wrap_${group_name}_ext.i"
    @ONLY IMMEDIATE)

  WRAP_ITK_INSTALL("/Configuration/Typedefs/java"
   "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/wrap_${group_name}_ext.i")

  SET(WRAP_ITK_JAVA_LIBRARY_DEPS ${WRAP_ITK_JAVA_LIBRARY_DEPS} "${java_file}" "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${base_name}_ext.i")

ENDMACRO(END_WRAP_MODULE_JAVA group_name)


###############################################################################
MACRO(WRAP_MODULE_JAVA module)

  SET(text "\n")
  
  SET(text "${text}%include java.i\n\n")
  
  SET(WRAP_ITK_JAVA_SWIG_EXT "${text}")
  
ENDMACRO(WRAP_MODULE_JAVA)


###############################################################################
MACRO(ADD_SIMPLE_TYPEDEF_JAVA wrap_class swig_name)

  IF("${wrap_class}" MATCHES "SmartPointer$")
    STRING(REGEX REPLACE "_Pointer$" "" smart_pointed "${swig_name}")
    ADD_JAVA_POINTER_TYPEMAP("${smart_pointed}")
  ENDIF("${wrap_class}" MATCHES "SmartPointer$")
  
ENDMACRO(ADD_SIMPLE_TYPEDEF_JAVA wrap_class swig_name)


###############################################################################
MACRO(ADD_JAVA_POINTER_TYPEMAP template_params)

  SET(text "DECLARE_REF_COUNT_CLASS_JAVA(${template_params})\n")
  SET(WRAP_ITK_JAVA_SWIG_EXT "${WRAP_ITK_JAVA_SWIG_EXT}${text}")
  
ENDMACRO(ADD_JAVA_POINTER_TYPEMAP template_params)


###############################################################################
MACRO(ADD_ONE_TYPEDEF_JAVA wrap_method wrap_class swig_name template_params)

  # store the wrapped final class names
  SET(WRAP_ITK_JAVA_CLASS_NAMES ${WRAP_ITK_JAVA_CLASS_NAMES} ${swig_name})

ENDMACRO(ADD_ONE_TYPEDEF_JAVA wrap_method wrap_class swig_name template_params)


###############################################################################
# Create the JavaUtils library

IF(NOT EXTERNAL_WRAP_ITK_PROJECT)  
  MACRO(END_WRAP_LIBRARIES_JAVA)
    ADD_SUBDIRECTORY(${WRAP_ITK_JAVA_SOURCE_DIR}/Tests)
#    ADD_SUBDIRECTORY(${WRAP_ITK_JAVA_SOURCE_DIR}/JavaUtils)
  ENDMACRO(END_WRAP_LIBRARIES_JAVA)

  MACRO(WRAP_LIBRARIES_JAVA)
    ADD_SUBDIRECTORY(${WRAP_ITK_JAVA_SOURCE_DIR}/JavaBase)
  ENDMACRO(WRAP_LIBRARIES_JAVA)

ELSE(NOT EXTERNAL_WRAP_ITK_PROJECT)
  MACRO(END_WRAP_LIBRARIES_JAVA)
    # just do nothing
  ENDMACRO(END_WRAP_LIBRARIES_JAVA)

  MACRO(WRAP_LIBRARIES_JAVA)
    # just do nothing
  ENDMACRO(WRAP_LIBRARIES_JAVA)

ENDIF(NOT EXTERNAL_WRAP_ITK_PROJECT)
