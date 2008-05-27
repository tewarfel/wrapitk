#SUBDIRS(Tests)

###############################################################################

SET(WRAP_ITK_JAVA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "java source dir")
SET(WRAP_ITK_JAVA_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "java binary dir")

# java stuff
FIND_PACKAGE(JAVA)
FIND_PACKAGE(JNI)
INCLUDE_DIRECTORIES(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})

# create the java directory in the classindex dir
FILE(MAKE_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java)

#!
SET(WRAP_ITK_JAVA_OUT_DIR ${PROJECT_BINARY_DIR}/Proxies/Java CACHE INTERNAL "")
SET(WRAP_ITK_JAVA_SRC_DIR ${WRAP_ITK_JAVA_OUT_DIR}/src CACHE INTERNAL "")
SET(WRAP_ITK_JAVA_BIN_DIR ${WRAP_ITK_JAVA_OUT_DIR}/bin CACHE INTERNAL "")
SET(WRAP_ITK_JAVA_JAR_DIR ${WRAP_ITK_JAVA_OUT_DIR}/jar CACHE INTERNAL "")

# java proxy package
SET(WRAP_ITK_JAVA_CORE_PACKAGE_NAME "org.itk" CACHE INTERNAL "")
SET(WRAP_ITK_JAVA_CORE_PACKAGE_PATH "org/itk" CACHE INTERNAL "") #TODO: this should be automated

# generate the swig external runtime header for this language
#SET(swig_externalruntime_header "swigExternalRuntimeJava.h")
#MESSAGE(STATUS "Generating swig external runtime header for Java: ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${swig_externalruntime_header}")
#EXEC_PROGRAM(${SWIG_EXECUTABLE}
#  ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}
#  ARGS
#  -java
#  -external-runtime ${swig_externalruntime_header}
#  )

# some glorbal variables
#SET(WRAP_ITK_JAVA_CLASS_NAMES "" CACHE INTERNAL "")

###############################################################################
MACRO(INIT_WRAP_LIBRARY_JAVA)
  CREATE_JAVA_DIRECTORY_STRUCTURE(WRAP_ITK_MODULES)
ENDMACRO(INIT_WRAP_LIBRARY_JAVA)


###############################################################################
MACRO(CREATE_JAVA_DIRECTORY_STRUCTURE modules_list)

  MESSAGE(STATUS "Creating wrapping Java packages directories.")
  
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_OUT_DIR})
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_SRC_DIR})
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_BIN_DIR})
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_JAR_DIR})

  # Packages output directory
  FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_SRC_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH})
  # Create the sub-package directories
  FOREACH(module ${${modules_list}})
    STRING(TOLOWER ${module} java_package_name)
    FILE(MAKE_DIRECTORY ${WRAP_ITK_JAVA_SRC_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH}/${java_package_name})
  ENDFOREACH(module ${${modules_list}})

ENDMACRO(CREATE_JAVA_DIRECTORY_STRUCTURE modules_list)


###############################################################################
MACRO(WRAP_LIBRARY_JAVA library_name)
#  SET(WRAP_ITK_JAVA_CONFIGURATION_TEMPLATES "")
#  SET(WRAP_ITK_JAVA_LIBRARY_MODULE "")
  SET(WRAP_ITK_JAVA_LIBRARY_DEPS )
#  SET(WRAP_ITK_JAVA_LIBRARY_DECLS )
#  SET(WRAP_ITK_JAVA_LIBRARY_CALLS )
  SET(WRAP_ITK_JAVA_CXX_FILES )
ENDMACRO(WRAP_LIBRARY_JAVA)


###############################################################################
MACRO(END_WRAP_LIBRARY_JAVA)

  INIT_WRAP_LIBRARY_JAVA() #! gets called more than once

  CONFIGURE_FILE("${WRAP_ITK_JAVA_SOURCE_DIR}/module_ext.i.in"
    "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${WRAPPER_LIBRARY_NAME}_ext.i"
    @ONLY IMMEDIATE)

  SET(interface_file "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${WRAPPER_LIBRARY_NAME}.i")
  SET(lib ${WRAPPER_LIBRARY_NAME}Java)
  SET(java_file "${LIBRARY_OUTPUT_PATH}/${lib}.java")
  SET(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${WRAPPER_LIBRARY_NAME}Java.cpp")
  
  STRING(TOLOWER ${WRAPPER_LIBRARY_NAME} java_package_name)
  SET(java_package_path ${WRAP_ITK_JAVA_SRC_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH}/${java_package_name})
  SET(java_package_name ${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${java_package_name})

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
    -o ${cpp_file}
    -I${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/Java
    -outdir ${java_package_path}
    ${interface_file}
    WORKING_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}
    DEPENDS ${DEPS} ${WRAP_ITK_JAVA_LIBRARY_DEPS} ${interface_file} ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${WRAPPER_LIBRARY_NAME}_ext.i ${WRAP_ITK_JAVA_SOURCE_DIR}/java.i ${SWIG_EXECUTABLE}
    COMMENT "Generating Java wrap files for class ${java_package_name}.${WRAPPER_LIBRARY_NAME}"
  )
  
  SET(lib ${WRAPPER_LIBRARY_NAME}Java)
  ADD_LIBRARY(${lib} MODULE ${cpp_file} ${WRAP_ITK_JAVA_CXX_FILES} ${WRAPPER_LIBRARY_CXX_SOURCES})
  SET_TARGET_PROPERTIES(${lib} PROPERTIES PREFIX "_")
  TARGET_LINK_LIBRARIES(${lib} ${WRAPPER_LIBRARY_LINK_LIBRARIES})
  FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS})
    ADD_DEPENDENCIES(${lib} ${dep}Swig)
  ENDFOREACH(dep)

ENDMACRO(END_WRAP_LIBRARY_JAVA)


###############################################################################
MACRO(END_INCLUDE_WRAP_CMAKE_JAVA group_name)
  
  SET(base_name wrap_${group_name})
    
  # create the swig interface for all the groups in the module
  #
  SET(interface_file "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/${base_name}.i")
  SET(lib ${group_name}Java)
  SET(java_file "${LIBRARY_OUTPUT_PATH}/${lib}.java")
  SET(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/${base_name}Java.cpp")

  SET(WRAP_ITK_JAVA_CXX_FILES ${WRAP_ITK_JAVA_CXX_FILES} ${cpp_file})

  STRING(TOLOWER ${WRAPPER_LIBRARY_NAME} java_package_name)
  SET(java_package_path ${WRAP_ITK_JAVA_SRC_DIR}/${WRAP_ITK_JAVA_CORE_PACKAGE_PATH}/${java_package_name})
  SET(java_package_name ${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${java_package_name})
  
  # prepare dependencies
  SET(DEPS )
  SET(WRAP_ITK_JAVA_TYPEMAP_JAVAIMPORTS )
  FOREACH(dep ${WRAPPER_LIBRARY_DEPENDS})
    # library dependencies
    SET(DEPS ${DEPS} ${${dep}SwigFiles})
    
    # create the import package commands
    STRING(TOLOWER ${dep} java_import_package_name)
    SET(java_import_package_name ${WRAP_ITK_JAVA_CORE_PACKAGE_NAME}.${java_import_package_name})
    SET(WRAP_ITK_JAVA_TYPEMAP_JAVAIMPORTS ${WRAP_ITK_JAVA_TYPEMAP_JAVAIMPORTS} "import ${java_import_package_name}.*;")
  ENDFOREACH(dep)
 
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
#      -l${WRAP_ITK_JAVA_SOURCE_DIR}/java.i
    -o ${cpp_file}
#      -I${WRAPPER_MASTER_INDEX_OUTPUT_DIR}
    -outdir ${java_package_path}
    ${interface_file}
    WORKING_DIRECTORY ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java
    DEPENDS ${DEPS} ${WRAP_ITK_JAVA_LIBRARY_DEPS} ${interface_file} ${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${base_name}_ext.i ${WRAP_ITK_JAVA_SOURCE_DIR}/java.i ${SWIG_EXECUTABLE}
    COMMENT "Generating Java wrap files for class ${java_package_name}.${group_name}"
  )
      
   CONFIGURE_FILE("${WRAP_ITK_JAVA_SOURCE_DIR}/module_ext.i.in"
    "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/wrap_${module}_ext.i"
    @ONLY IMMEDIATE)

  SET(WRAP_ITK_JAVA_LIBRARY_DEPS ${WRAP_ITK_JAVA_LIBRARY_DEPS} "${java_file}" "${WRAPPER_MASTER_INDEX_OUTPUT_DIR}/java/${base_name}_ext.i")

ENDMACRO(END_INCLUDE_WRAP_CMAKE_JAVA group_name)


###############################################################################
MACRO(INCLUDE_WRAP_CMAKE_JAVA module)

  SET(text "\n")
  
  SET(text "${text}%include ${WRAP_ITK_JAVA_SOURCE_DIR}/java.i\n\n")
  
  SET(WRAP_ITK_JAVA_SWIG_EXT "${text}")
  
ENDMACRO(INCLUDE_WRAP_CMAKE_JAVA)


###############################################################################
MACRO(ADD_SIMPLE_TYPEDEF_JAVA wrap_class swig_name)

  # store the wrapped class names, these classes hold the template parameters
#  SET(WRAP_ITK_JAVA_CLASS_NAMES ${WRAP_ITK_JAVA_CLASS_NAMES} ${swig_name})

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

